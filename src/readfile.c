#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "perrorf.h"

static const char *str = "8\n"
"-1.0 -1.0  1.0 -1.0 -1.0  1.0  0.0 0.0\n"
"-1.0  1.0  1.0  1.0 -1.0  1.0  0.0 0.5\n"
" 1.0  1.0  1.0  1.0  1.0  1.0  0.0 0.5\n"
" 1.0 -1.0  1.0 -1.0  1.0  1.0  1.0 0.0\n"
"-1.0 -1.0 -1.0 -1.0 -1.0 -1.0  0.0 0.5\n"
"-1.0  1.0 -1.0  1.0 -1.0 -1.0  0.0 1.0\n"
" 1.0  1.0 -1.0  1.0  1.0 -1.0  1.0 1.0\n"
" 1.0 -1.0 -1.0 -1.0  1.0 -1.0  1.0 0.5\n"
"36\n"
"0 1 2\n"
"2 3 0 \n"
"1 5 6\n"
"6 2 1\n"
"7 6 5\n"
"5 4 7\n"
"4 0 3\n"
"3 7 4\n"
"4 5 1\n"
"1 0 4\n"
"3 2 6\n"
"6 7 3\n";

char *readfd(int fd)
{
	int res;
	off_t len;
	void *data;
	char *str = NULL;

	len = lseek(fd, 0, SEEK_END);
	if (len == (off_t)-1) {
		perror("lseek");
		goto error1;
	}

	if (len == 0)
		goto error1;

	data = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
	if (data == MAP_FAILED) {
		perror("mmap");
		goto error1;
	}

	str = malloc(len + 1);
	if (str == NULL) {
		perror("malloc");
		goto error2;
	}

	strncpy(str, data, len);
	str[len] = '\0';

error2:
	res = munmap(data, len);
	if (res == -1)
		perror("munmap");
error1:
	return str;
}

char *readfile(const char *filename)
{
	int fd, res;
	char *str = NULL;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perrorf("open(%s)", filename);
		goto error1;
	}

	str = readfd(fd);
	if (str == NULL) {
		goto error2;
	}

error2:
	res = close(fd);
	if (res == -1)
		perror("close");
error1:
	return str;
}
