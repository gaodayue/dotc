#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static inline void
err_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

static void
sig_int(int signo)
{
	write(STDOUT_FILENO, "handle SIGINT\n", 14);
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		err_exit("signal SIGINT");
}

/*
 * $ ./a.out
 * SIGINT blocked!			(1)
 * ^C^CSIGINT is pending	(2)
 * handle SIGINT			(3)
 * SIGINT unblocked!		(4)
 * ^C
 *
 * (3) is before (4) because before returning from sigprocmask, 
 * at least one of the unblocked pending signal is delivered.
 */
int
main(void)
{
	sigset_t	new_sigset,
				old_sigset,
				pend_sigset;
	
	/* install SIGINT handler */
	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_exit("error signal SIGINT");

	/* block SIGINT */
	sigemptyset(&new_sigset);
	sigaddset(&new_sigset, SIGINT);
	if (sigprocmask(SIG_BLOCK, &new_sigset, &old_sigset))
		err_exit("error sigprocmask");
	printf("SIGINT blocked!\n");

	sleep(5);	/* SIGINT won't be delivered at this time */

	if (sigpending(&pend_sigset))
		err_exit("error sigpending");
	if (sigismember(&pend_sigset, SIGINT))
		printf("SIGINT is pending\n");

	/* unblock SIGINT */
	if (sigprocmask(SIG_SETMASK, &old_sigset, NULL))
		err_exit("error sigprocmask");
	printf("SIGINT unblocked!\n");

	sleep(5);	/* SIGINT should interrupt program */
	exit(EXIT_SUCCESS);
}
