#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void * kmemset(void *, int, size_t);
void * kmemcpy(void *, const void *, size_t);
void * kmemmove(void *, const void *, size_t);

#endif
