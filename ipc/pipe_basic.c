#include <stdio.h>
#include <unistd.h>

#define MAXBUFSIZE 1024

int main(void)
{
	int fds[2];
	pid_t pid;
	char buf[MAXBUFSIZE];

	// create pipe for parent writing message to child
	if (pipe(fds) == -1)
	{
		perror("cann't create pipe");
		return 1;
	}

	if ((pid = fork()) < 0)
	{
		perror("cann't fork new process");
		return 1;
	}
	else if (pid > 0)
	{
		close(fds[0]);
		write(fds[1], "hello!", 6);
	}
	else
	{
		close(fds[1]);
		read(fds[0], buf, MAXBUFSIZE);
		printf("child read from parent: %s\n", buf);
	}
	return 0;
}
