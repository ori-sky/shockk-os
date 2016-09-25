#include <kernel/memory.h>

void * kmemset(void *dst, int c, size_t count) {
	unsigned char *ptr = (unsigned char *)dst;
	while(count--) { *ptr++ = (unsigned char)c; }
	return dst;
}

void * kmemcpy(void *dst, const void *src, size_t count) {
	if(dst == src) { return dst; }

	unsigned char *p_dst = (unsigned char *)dst;
	const unsigned char *p_src = (const unsigned char *)src;

	while(count--) {
		*p_dst++ = *p_src++;
	}

	return dst;
}

void * kmemmove(void *dst, const void *src, size_t count) {
	if(dst == src) { return dst; }

	unsigned char *p_dst = (unsigned char *)dst;
	const unsigned char *p_src = (const unsigned char *)src;

	if(dst < src) {
		p_dst = (unsigned char *)dst;
		p_src = (const unsigned char *)src;
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
