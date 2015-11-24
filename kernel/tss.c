#include <kernel/memory.h>
#include <kernel/tss.h>

void tss_init(struct TSS *tss) {
	for(unsigned int i = 0; i < sizeof(struct TSS); ++i) {
		((unsigned char *)tss)[i] = 0;
	}
	tss->ss0 = 0x10;
	tss->esp0 = kmalloc(1 << 20) + (1 << 20);
	__asm__ ("ltr %0" : : "r" ((uint16_t)0x2B)); /* 0x28 (5th segment) OR'd with 0x3 (ring 3) */
}
