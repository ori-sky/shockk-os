#include <stdint.h>
#include "ports.h"
#include "pic.h"
#include "idt.h"
#include "irq.h"

void entry(void) {
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	volatile struct IDT *idt = (struct IDT *)0x500;
	idt_init(idt);

	__asm__ __volatile__ ("lidt (%0)" : : "r" (&idt->descriptor));
	__asm__ __volatile__ ("sti");

	volatile unsigned char *ptr = (unsigned char *)0xB8000;
	for(unsigned short col = 0; col < 80 * 25; ++col) {
		ptr[col << 1] = 'A' + col % 80 % 26;
	}

	for(;;) { __asm__ __volatile__ ("hlt"); }
}
