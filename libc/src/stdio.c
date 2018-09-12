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

int putc(int i, FILE *stream) {
	char c = i;
	int ret = write(stream->descriptor, &c, 1);
	return ret;
}

int putchar(int i) {
	return putc(i, stdout);
}
