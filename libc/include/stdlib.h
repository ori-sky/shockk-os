#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stdbool.h>
#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

int atoi(const char *);
long int strtol(const char * restrict, char ** restrict, int);
long long int strtoll(const char * restrict, char ** restrict, int);
unsigned long long int strtoull(const char * restrict, char ** restrict, int);

void free(void *);
void * malloc(size_t);
void * realloc(void *, size_t);

_Noreturn void abort(void);
_Noreturn void exit(int);
_Noreturn void _Exit(int);

void qsort(void *, size_t, size_t, int (*)(const void *, const void *));

#endif
