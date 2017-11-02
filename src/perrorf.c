#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "perrorf.h"

void perrorf(char *fmt, ...)
{
	int res;
	char *str, *errdesc;
	va_list args;
	int errnum;

	errnum = errno;
	errdesc = strerror(errnum);

	va_start(args, fmt);
	res = vasprintf(&str, fmt, args);
	if (res == -1) {
		perror("vasprintf (during perrorf)");
		fprintf(stderr, "%s: %s\n", fmt, errdesc);
	} else {
		fprintf(stderr, "%s: %s\n", str, errdesc);
	}
	va_end(args);
}
