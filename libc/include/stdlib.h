#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stdbool.h>
#include <stddef.h>

void free(void *);
void * malloc(size_t);

_Noreturn void exit(int);

void qsort(void *, size_t, size_t, int (*)(const void *, const void *));

#endif
