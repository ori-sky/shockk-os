#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

FILE _stdin  = {STDIN_FILENO,  0, 0};
FILE _stdout = {STDOUT_FILENO, 0, 0};
FILE _stderr = {STDERR_FILENO, 0, 0};

FILE *stdin  = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

int fflush(FILE *stream) {
	return 0;
}

FILE * fopen(const char * restrict filename, const char * restrict mode) {
	int filedes = open(filename, 0);
	if(filedes == -1) { return NULL; }
	return NULL; // XXX
}

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
	return fputs(format, stream);
}

int fgetc(FILE *stream) {
	unsigned char c;
	size_t ret = fread(&c, 1, 1, stream);
	return ret == 1 ? c : EOF;
}

char * fgets(char * restrict s, int n, FILE * restrict stream) {
	size_t i;
	for(i = 0; i < n - 1; ++i) {
		size_t ret = fread(&s[i], 1, 1, stream);
		if(ret < 1) { return NULL; }
		if(s[i] == '\n') { break; }
	}
	s[i] = '\0';
	return s;
}

int fputc(int i, FILE *stream) {
	char c = i;
	int ret = write(stream->descriptor, &c, 1);
	return ret >= 0 ? i : EOF;
}

int fputs(const char * restrict s, FILE * restrict stream) {
	size_t len = strlen(s);
	size_t ret = fwrite(s, 1, len, stream);
	return ret == len ? ret : EOF;
}

int getc(FILE *stream) {
	return fgetc(stream);
}

int getchar(void) {
	return getc(stdin);
}

int putc(int i, FILE *stream) {
	return fputc(i, stream);
}

int putchar(int i) {
	return putc(i, stdout);
}

int puts(const char *s) {
	int ret = fputs(s, stdout);
	if(ret == EOF) { return EOF; }
	int ret2 = putchar('\n');
	if(ret2 == EOF) { return EOF; }
	return ret + 1;
}

size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream) {
	char *sz;
	size_t memb;
	for(memb = 0, sz = ptr; memb < nmemb; ++memb, sz += size) {
		int ret = read(stream->descriptor, sz, size);
		if(ret < 0) { break; }
	}
	return memb;
}

size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream) {
	const char *sz;
	size_t memb;
	for(memb = 0, sz = ptr; memb < nmemb; ++memb, sz += size) {
		int ret = write(stream->descriptor, sz, size);
		if(ret < 0) { break; }
	}
	return memb;
}
