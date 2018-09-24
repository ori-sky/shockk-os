#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H 1

#include <sys/types.h>

#define PROT_NONE  0
#define PROT_READ  1
#define PROT_WRITE 2
#define PROT_EXEC  4

#define MAP_PRIVATE 0
#define MAP_SHARED  1
#define MAP_FIXED   2

#define MAP_FAILED ((void *)0)

void * mmap(void *, size_t, int, int, int, off_t);

#endif
