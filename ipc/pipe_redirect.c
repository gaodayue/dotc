#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

#define MAXBUFSIZE 1024

static inline void
err_exit(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, ": %s\n", strerror(errno));
	exit(1);
}

static void
do_parent(pid_t childpid, int fds[], char *fpath)
{
	int		infile_fd, n;
	char	buf[MAXBUFSIZE];

	close(fds[0]);	/* don't need read end of the pipe */

	if ((infile_fd = open(fpath, O_RDONLY)) == -1)
		err_exit("fail to open file '%s'", fpath);

	while ((n = read(infile_fd, buf, MAXBUFSIZE)) > 0)
		if (write(fds[1], buf, n) != n)
			err_exit("fail to write pipe");

	if (n == -1)
		err_exit("fail to read file '%s'", fpath);
	close(infile_fd);
	close(fds[1]);

	if (waitpid(childpid, NULL, 0) == -1)
		err_exit("fail to waitpid");
}

static void
do_child(int fds[])
{
	close(fds[1]);	/* don't need write end of the pipe */
	/* redirect stdin to read from pipe */
	if (fds[0] != STDIN_FILENO)
	{
		if (dup2(fds[0], STDIN_FILENO) != STDIN_FILENO)
			err_exit("fail to dup2 file descriptor");
		close(fds[0]);
	}

	/* call "less" to pager stdin */
	execlp("less", "less", (char *) 0);
	err_exit("fail to call execlp");
}

int
main(int argc, char **argv)
{
	int		fds[2];
	pid_t	pid;

	if (argc != 2)
	{
		printf("Usage: %s <file>\n", argv[0]);
		exit(0);
	}

	if (pipe(fds) == -1)
		err_exit("fail to create pipe");

	if ((pid = fork()) < 0)
		err_exit("fail to fork");
	else if (pid == 0)
		do_child(fds);
	else
		do_parent(pid, fds, argv[1]);

	exit(0);
}
