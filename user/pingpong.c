#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
main()
{
	int p[2];
	char buff[5];
	pipe(p);
	if(fork() == 0)
	{
		read(p[0], buff, 5);
		printf("%d: received ping\n", getpid());
		write(p[1], "pong", 4);
	}
	else
	{
		write(p[1], "ping", 4);
		wait(0);
		read(p[0], buff, 5);
		printf("%d: received pong\n", getpid());
	}
}