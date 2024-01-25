#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
main()
{
	int p1[2], p2[2];
	pipe(p1);
	pipe(p2);
	char buff[5];
	int pid = fork();
	if (pid == 0) {
		close(p1[1]);
		close(p2[0]);
		read(p1[0], buff, 5);
		printf("%d: received ", getpid());
		for (int i = 0; i < 4; i++) {
			printf("%c", buff[i]);
		}
		printf("\n");
		write(p2[1], "pong", 4);
		close(p1[0]);
		close(p2[1]);
		exit(0);
	} else {
		close(p1[0]);
		close(p2[1]);
		write(p1[1], "ping", 4);
		wait(0);
		read(p2[0], buff, 5);
		printf("%d: received ", getpid());
		for (int i = 0; i < 4; i++) {
			printf("%c", buff[i]);
		}
		printf("\n");
		close(p1[1]);
		close(p2[0]);
		exit(0);
	}
	
}