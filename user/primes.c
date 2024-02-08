#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int *p) {
    int pid = fork();
    if (pid > 0) {
        wait(0);
    } else if (pid == 0) {
        int pr;
        if (read(p[0], &pr, 4) == 4) {
            printf("prime %d\n", pr);
            int n;
            int new_p[2];
            pipe(new_p);
            while (read(p[0], &n, 4) == 4) {
                if (n % pr) {
                    write(new_p[1], &n, 4);
                }
            }
            close(new_p[1]);
            prime(new_p);
        }
    }
}

int main() {
    int p[2];

    pipe(p);
    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, 4);        
    }
    close(p[1]);
    prime(p);

    return 0;
}