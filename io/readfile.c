#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define fail(...) failIf(true, __VA_ARGS__)

#define ERR_USAGE	1
#define ERR_IO 		2
#define ERR_OOM		4

static inline void failIf(bool condition, int exitCode, char* fmt, ...) {
	if (!condition) return;
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(exitCode);
}

char* readFile(char* path) {
	FILE* file = fopen(path, "r");
	failIf(file == NULL, ERR_IO, "could not to open file \"%s\".\n", path);

	/* Find out how big the file is. */
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	/* Allocate a buffer for file content. */
	char* buffer = malloc(fileSize + 1);
	failIf(buffer == NULL, ERR_OOM, "OutOfMemory!\n");

	/* Read the entire file. */
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	failIf(bytesRead < fileSize, ERR_IO, "could not read file \"%s\"\n", path);

	buffer[bytesRead] = '\0';
	fclose(file);
	return buffer;
}

int main(int argc, char** argv) {
	if (argc != 2)
	{
		fail(ERR_USAGE, "Usage: %s <file>\n", argv[0]);
	}

	char* source = readFile(argv[1]);
	puts(source);
	free(source);
	return 0;
}
