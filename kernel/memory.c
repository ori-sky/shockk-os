#include <kernel/memory.h>

static unsigned char *head = (unsigned char *)(1 << 20);

void * kmalloc(size_t size) {
	void *ptr = head;
	head += size;
	return ptr;
}