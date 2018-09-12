#ifndef _STDIO_H
#define _STDIO_H 1

#include <stdarg.h>
#include <stddef.h>

typedef size_t fdesc_t;
typedef size_t fpos_t;

typedef struct {
	fdesc_t descriptor;
	fpos_t position;
	void *buffer;
} FILE;

FILE _stdin  = {0, 0, 0};
FILE _stdout = {1, 0, 0};
FILE _stderr = {2, 0, 0};

FILE *stdin  = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

int fprintf(FILE * restrict, const char * restrict, ...);
int printf(const char * restrict, ...);
int vfprintf(FILE * restrict, const char * restrict, va_list);

int putc(int, FILE *);
int putchar(int);

#endif
