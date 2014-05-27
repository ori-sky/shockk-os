#include <ports.h>

unsigned char ports_inb(unsigned short port)
{
	unsigned char result;
	__asm__ volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void ports_outb(unsigned short port, unsigned char data)
{
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}
