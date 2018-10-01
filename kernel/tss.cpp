#include <kernel/tss.h>

void tss_init(struct TSS *tss, Pager *pager) {
	unsigned char *stack = (unsigned char *)pager->GetContext().Reserve();
	tss->esp0 = (uint32_t)&stack[PAGE_ALLOCATOR_PAGE_SIZE];
	tss->ss0 = 0x10;
	__asm__ ("ltr %0" : : "r" ((uint16_t)0x2B)); /* 0x28 (5th segment) OR'd with 0x3 (ring 3) */
}
