#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1024

int main(int argc, char **argv)
{
	FILE	*infile,
			*pipe;
	char	buf[MAXLINE];

	if (argc != 2)
	{
		printf("Usage: %s <file>\n", argv[0]);
		exit(0);
	}

	if ((pipe = popen("less", "w")) == NULL)
	{
		perror("popen failed!");
		exit(1);
	}

	if ((infile = fopen(argv[1], "r")) == NULL)
	{
		perror("file to open file");
		exit(1);
	}

	/* read from input file, write to pipe */
	while (fgets(buf, MAXLINE, infile) != NULL)
		if (fputs(buf, pipe) == EOF)
		{
			perror("output error");
			exit(1);
		}

	if (ferror(infile))
	{
		perror("input error");
		exit(1);
	}

	fclose(infile);
	pclose(pipe);
}
