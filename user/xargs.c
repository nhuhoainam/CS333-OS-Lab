#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char*argv[]) {
    char ch;
    char* xargv[MAXARG];
    char buf[512], *p = buf;
    int xargc = argc;
    for (int i = 0; i < argc; i++)
        xargv[i] = argv[i + 1];
    while (read(0, &ch, 1) == 1) {
        if (ch == ' ') {
            xargv[xargc++ - 1] = buf;
            memset(buf, 0, sizeof(buf));
            p = buf;
            continue;
        }
        if (ch == '\n') {
            xargv[xargc++ - 1] = buf;
            int pid = fork();
            if (pid > 0) {
                wait(0);
                memset(buf, 0, sizeof(buf));
                p = buf;
                xargc = argc;
                for (int i = argc; i < MAXARG; i++) {
                    xargv[i] = 0;
                }
                continue;
            } else {
                exec(xargv[0], xargv);
            }
        }
        *p++ = ch;
    }

    return 0;
}