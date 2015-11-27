#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void * kmalloc(size_t);
void * memset(void *, int, size_t);
void * memcpy(void *, const void *, size_t);
void * memmove(void *, const void *, size_t);

#endif
