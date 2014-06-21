#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAXLINE 1000
#define MAXARGS 128

extern char **environ; /* defined by libc */

void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

void unix_error(char *msg)
{
	fprintf(stderr, "unix error: %s\n", strerror(errno));
	exit(1);
}

int main()
{
	char cmdline[MAXLINE];

	while (1) {
		/* Read */
		printf("tiny> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);

		/* Evaluate */
		eval(cmdline);
	}
}

/* eval - Evaluate a command line */
void eval(char *cmdline)
{
	char *argv[MAXARGS]; /* argument lists to execve() */
	char buf[MAXLINE];	 /* holds modified command line */
	int bg;				 /* should the job run in bg or fg? */
	pid_t pid;			 /* process id */

	strcpy(buf, cmdline);
	bg = parseline(buf, argv);
	if (argv[0] == NULL)
		return; /* ignore empty line */

	if (!builtin_command(argv)) {
		if ((pid = fork()) == 0) { /* child runs the job */
			if (execve(argv[0], argv, environ) < 0) {
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
		}

		/* parent waits for foreground job to terminate */
		if (!bg) {
			int stat;
			if (waitpid(pid, &stat, 0) < 0)
				unix_error("waitfg: waitpid error");
		} else {
			printf("%d %s\n", pid, cmdline);
		}
	}
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv)
{
	if (strcmp(argv[0], "quit") == 0)	/* quit command */
		exit(0);
	if (strcmp(argv[0], "&") == 0)		/* ignore single & */
		return 1;
	return 0;
}

/* Parse the command line and build the argv array */
int parseline(char *buf, char **argv)
{
	char *delim;	/* points to first space delim */
	int argc;
	int bg;

	buf[strlen(buf) - 1] = ' ';	/* replace trailing '\n' with space */
	while (*buf && (*buf == ' '))	/* ignore leading spaces */
		buf++;

	/* build the argv list */
	argc = 0;
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf != ' '))
			buf++;
	}
	argv[argc] = NULL;

	if (argc == 0)	/* ignore blank line */
		return 1;

	/* should the job run in the background? */
	bg = *argv[argc - 1] == '&';
	if (bg)
		argv[--argc] = NULL;

	return bg;
}
