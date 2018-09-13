#ifndef _STDIO_H
#define _STDIO_H 1

#include <stdarg.h>
#include <stddef.h>

typedef size_t fpos_t;

typedef size_t _fdesc_t;
typedef struct {
	_fdesc_t descriptor;
	fpos_t position;
	void *buffer;
} FILE;

#define EOF (-1)

#define SEEK_CUR 0
#define SEEK_END 1
#define SEEK_SET 2

FILE *stdin;
FILE *stdout;
FILE *stderr;

int fflush(FILE *);

int fprintf(FILE * restrict, const char * restrict, ...);
int printf(const char * restrict, ...);
int vfprintf(FILE * restrict, const char * restrict, va_list);

int fputc(int, FILE *);
int fputs(const char * restrict, FILE * restrict);
int putc(int, FILE *);
int putchar(int);
int puts(const char *);

size_t fwrite(const void * restrict, size_t, size_t, FILE * restrict);

#endif
