#ifndef _STDIO_H
#define _STDIO_H 1

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#define restrict
extern "C" {
#endif

typedef size_t fpos_t;

enum file_type_t {
	FILE_TYPE_FD,
	FILE_TYPE_MEM
};

typedef size_t _fdesc_t;
typedef struct {
	enum file_type_t type;
	_fdesc_t descriptor;
	void *buffer;
	size_t size;
	fpos_t position;
} FILE;

#define BUFSIZ 256

#define EOF (-1)

#define SEEK_CUR 0
#define SEEK_END 1
#define SEEK_SET 2

FILE *stdin;
FILE *stdout;
FILE *stderr;

int fclose(FILE *);
int fflush(FILE *);
FILE * fmemopen(void * restrict, size_t, const char * restrict);
FILE * fopen(const char * restrict, const char * restrict);

int fprintf(FILE * restrict, const char * restrict, ...);
int printf(const char * restrict, ...);
int snprintf(char * restrict, size_t, const char * restrict, ...);
int sprintf(char * restrict, const char * restrict, ...);
int vfprintf(FILE * restrict, const char * restrict, va_list);
int vprintf(const char * restrict, va_list);
int vsnprintf(char * restrict, size_t, const char * restrict, va_list);
int vsprintf(char * restrict, const char * restrict, va_list);

int fgetc(FILE *);
char * fgets(char * restrict, int, FILE * restrict);
int fputc(int, FILE *);
int fputs(const char * restrict, FILE * restrict);
int getc(FILE *);
int getchar(void);
int putc(int, FILE *);
int putchar(int);
int puts(const char *);

void clearerr(FILE *);
int ferror(FILE *);
size_t fread(void * restrict, size_t, size_t, FILE * restrict);
size_t fwrite(const void * restrict, size_t, size_t, FILE * restrict);

#ifdef __cplusplus
}
#endif

#endif
