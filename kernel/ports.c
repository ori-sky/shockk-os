#include <kernel/ports.h>

uint8_t ports_inb(unsigned short port) {
	uint8_t result;
	__asm__ __volatile__ ("in %1, %0" : "=a" (result) : "d" (port));
	return result;
}

uint32_t ports_inl(unsigned short port) {
	uint32_t result;
	__asm__ __volatile__ ("in %1, %0" : "=a" (result) : "d" (port));
	return result;
}

void ports_outb(unsigned short port, uint8_t data) {
	__asm__ ("out %0, %1" : : "a" (data), "d" (port));
}

void ports_outl(unsigned short port, uint32_t data) {
	__asm__ ("out %0, %1" : : "a" (data), "d" (port));
}
