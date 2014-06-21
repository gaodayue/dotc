#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CHILD 4

void unix_error(char *msg)
{
	fprintf(stderr, "unix error: %s\n", strerror(errno));
	exit(1);
}

int main(void)
{
	pid_t pid;

	// create NUM_CHILD processes
	for (int i = 0; i < NUM_CHILD; i++)
	{
		if (fork() == 0) /* child */
		{
			exit(100 + i);
		}
	}

	// parent reaps children in no particular order
	int stat;
	while ((pid = waitpid(-1, &stat, 0)) > 0) {
		if (WIFEXITED(stat))
			printf("child %d terminates normally with exit code %d\n",
				   pid, WEXITSTATUS(stat));
		else
			printf("child %d terminates abnormally\n", pid);
	}

	if (errno != ECHILD)
		unix_error("waitpid error");
	
	exit(0);
}
