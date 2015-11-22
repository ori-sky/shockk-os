#include <kernel/ports.h>

unsigned char ports_inb(unsigned short port) {
	unsigned char result;
	__asm__ __volatile__ ("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void ports_outb(unsigned short port, unsigned char data) {
	__asm__ __volatile__ ("out %%al, %%dx" : : "a" (data), "d" (port));
}
