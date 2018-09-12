#include <stdio.h>
#include <unistd.h>

int fprintf(FILE * restrict stream, const char * restrict format, ...) {
	va_list arg;

	va_start(arg, format);
	int ret = vfprintf(stream, format, arg);
	va_end(arg);

	return ret;
}

int printf(const char * restrict format, ...) {
	va_list arg;

	va_start(arg, format);
	int ret = vfprintf(stdout, format, arg);
	va_end(arg);

	return ret;
}

int vfprintf(FILE * restrict stream, const char * restrict format, va_list arg) {
	int ret = write(stream->descriptor, format, strlen(format));
	return ret;
}
