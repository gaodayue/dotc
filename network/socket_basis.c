#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv)
{
	int		fd;
	struct sockaddr_in	sin;
	struct hostent		*h;
	size_t	len;
	ssize_t	ret;
	char	buf[1024];

	/* get address for the target host */
	char *targethost = "www.baidu.com";
	h = gethostbyname(targethost);
	if (!h)
	{
		fprintf(stderr, "Couldn't lookup %s: %s\n", targethost, hstrerror(h_errno));
		exit(EXIT_FAILURE);
	}
	if (h->h_addrtype != AF_INET)
	{
		fprintf(stderr, "IPV6 is not supported.");
		exit(EXIT_FAILURE);
	}

	/* create a new socket */
	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* connect to remote host */
	sin.sin_family	= AF_INET;
	sin.sin_port	= htons(80);
	sin.sin_addr	= * (struct in_addr *) h->h_addr;
	if (connect(fd, (struct sockaddr *) &sin, sizeof(sin)) == -1)
	{
		perror("connect");
		close(fd);
		exit(EXIT_FAILURE);
	}

	/* send request */
	char *query =
		"GET / HTTP/1.0\r\n"
		"Host: www.baidu.com\r\n"
		"\r\n";
	len = strlen(query);
	while (len)
	{
		ret = write(fd, query, len);
		if (ret <= 0)
		{
			perror("write");
			close(fd);
			exit(EXIT_FAILURE);
		}
		len		-= ret;
		query	+= ret;
	}

	/* get response and print out */
	while ((ret = read(fd, buf, sizeof(buf))) > 0)
	{
		fwrite(buf, sizeof(char), ret, stdout);
	}
	if (ret == -1)
	{
		perror("read");
		close(fd);
		exit(EXIT_FAILURE);
	}

	close(fd);
	exit(EXIT_SUCCESS);
}
