#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sig_alrm(int);
static void sig_int(int);

int main(void)
{
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
	{
		perror("signal SIGALRM");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGINT, sig_int) == SIG_ERR)
	{
		perror("signal SIGINT");
		exit(EXIT_FAILURE);
	}
	alarm(1);
	while (1) ;
}

static void sig_alrm(int signo)
{
	alarm(1);
	write(STDOUT_FILENO, "in signal\n", 10);
}

static void sig_int(int signo)
{
	write(STDOUT_FILENO, "in sigint\n", 10);
}
