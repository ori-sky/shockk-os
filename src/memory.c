#include <kernel/memory.h>

static unsigned char *head = (unsigned char *)0x100000; /* 1M */

void * kmalloc(size_t size) {
	void *ptr = head;
	head += size;
	return ptr;
}

void * memset(void *dst, int c, size_t count) {
	unsigned char *ptr = dst;
	while(count--) { *ptr++ = (unsigned char)c; }
	return dst;
}

void * memcpy(void *dst, const void *src, size_t count) {
	if(dst == src) { return dst; }

	unsigned char *p_dst = dst;
	const unsigned char *p_src = src;

	while(count--) {
		*p_dst++ = *p_src++;
	}

	return dst;
}

void * memmove(void *dst, const void *src, size_t count) {
	if(dst == src) { return dst; }

	unsigned char *p_dst = dst;
	const unsigned char *p_src = src;

	if(dst < src) {
		p_dst = dst;
		p_src = src;
		while(count--) {
			*p_dst++ = *p_src++;
		}
	} else {
		p_dst = &p_dst[count];
		p_src = &p_src[count];
		while(count--) {
			*p_dst-- = *p_src--;
		}
	}

	return dst;
}
