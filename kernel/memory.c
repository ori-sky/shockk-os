#include <kernel/memory.h>

static unsigned char *head = (unsigned char *)0x100000; /* 1M */

void * kmalloc(size_t size) {
	void *ptr = head;
	head += size;
	return ptr;
}
