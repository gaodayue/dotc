#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define TIMEOUT 5		/* select timeout in seconds */
#define BUF_LEN 1024

/*
 * This example blocks waiting for input on stdin for up
 * to 5 seconds. It's not actually multiplexing I/O.
 */
int main(void)
{
	struct timeval tv;
	fd_set readfds;
	int ret;

	/* Wait on stdin for input */
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	/* Wait up to 5 seconds */
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;

	/* Start block! */
	ret = select(STDIN_FILENO + 1,
				 &readfds,
				 NULL,
				 NULL,
				 &tv);
	if (ret == -1) {
		perror("select");
		return 1;
	} else if (!ret) {
		printf("%d seconds elapsed.\n", TIMEOUT);
		return 0;
	}

	/*
	 * Is out file descriptor ready to read?
	 * It must be, as it was the only fd we provide,
	 * and the call returns nonzero.
	 */
	if (FD_ISSET(STDIN_FILENO, &readfds)) {
		char buf[BUF_LEN+1];
		int len;
		
		/* guaranteed to not block */
		if ((len = read(STDIN_FILENO, buf, BUF_LEN)) == -1) {
			perror("read");
			return 1;
		}

		if (len) {
			buf[len] = '\0';
			printf("read: %s", buf);
		}

		return 0;
	}

	fprintf(stderr, "should not get here!\n");
	return 1;
}
