#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H 1

enum {
	PROT_EXEC,
	PROT_NONE,
	PROT_READ,
	PROT_WRITE
};

enum {
	MAP_FIXED,
	MAP_PRIVATE,
	MAP_SHARED
};

#define MAP_FAILED ((void *)0)

void * mmap(void *, size_t, int, int, int, off_t);

#endif
