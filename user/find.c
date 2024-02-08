#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char* path, char* matcher) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat outer %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_DEVICE:
    case T_FILE:
        close(fd);
        break;

    case T_DIR:
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
            if (de.inum == 0)
                continue;
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (strcmp(de.name, matcher) == 0) {
                printf("%s\n", buf);
            }
            find(buf, matcher);
        }
        close(fd);
        break;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        exit(0);
    }
    for (int i = 2; i < argc; i++) {
        find(argv[1], argv[i]);
    }
    return 0;
}