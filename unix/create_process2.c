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
	pid_t pids[NUM_CHILD], retpid;

	// create NUM_CHILD processes
	for (int i = 0; i < NUM_CHILD; i++)
		if ((pids[i] = fork()) == 0) /* child */
		{
			sleep(10);
			exit(100 + i);
		}

	// parent reaps children in order
	int stat, i;
	i = 0;
	while ((retpid = waitpid(pids[i++], &stat, 0)) > 0)
		if (WIFEXITED(stat))
			printf("child %d terminates normally with exit code %d\n",
				   retpid, WEXITSTATUS(stat));
		else
			printf("child %d terminates abnormally\n", retpid);

	if (errno != ECHILD)
		unix_error("waitpid error");
	
	exit(0);
}
