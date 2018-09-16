#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stddef.h>

void free(void *);
void * malloc(size_t);

_Noreturn void exit(int);

#endif
