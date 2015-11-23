#include <kernel/ports.h>

unsigned char ports_inb(unsigned short port) {
	unsigned char result;
	__asm__ __volatile__ ("in %1, %0" : "=a" (result) : "d" (port));
	return result;
}

void ports_outb(unsigned short port, unsigned char data) {
	__asm__ __volatile__ ("out %0, %1" : : "a" (data), "d" (port));
}
