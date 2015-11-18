#include <stdint.h>
#include "ports.h"
#include "pic.h"
#include "idt.h"

#define IRQ0 0x20        /* programmable interval timer */
#define IRQ1 (IRQ0 + 1)  /* keyboard */
#define IRQ2 (IRQ0 + 2)
#define IRQ3 (IRQ0 + 3)
#define IRQ4 (IRQ0 + 4)
#define IRQ5 (IRQ0 + 5)
#define IRQ6 (IRQ0 + 6)
#define IRQ7 (IRQ0 + 7)
#define IRQ8 0x28
#define IRQ9  (IRQ8 + 1)
#define IRQ10 (IRQ8 + 2)
#define IRQ11 (IRQ8 + 3)
#define IRQ12 (IRQ8 + 4)
#define IRQ13 (IRQ8 + 5)
#define IRQ14 (IRQ8 + 6)
#define IRQ15 (IRQ8 + 7)

struct CPUState {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t interrupt;
	uint32_t error;
};

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

inline char * uitoa(unsigned int value, char *str, unsigned int base) {
	unsigned char log = 0;
	for(unsigned int n=value; n>=base; n/=base) { ++log; }

	str[log+1] = 0;

	unsigned short div = 1;
	for(unsigned char i=log; i!=(unsigned char)(-1); --i, div*=base) {
		unsigned char offset = value / div % base;
		unsigned char base = offset < 10 ? '0' : 'a' - 10;
		str[i] = base + offset;
	}

	return str;
}

void isr_main(struct CPUState cpu_state) {
	if(cpu_state.interrupt == IRQ7) { return; }
	if(cpu_state.interrupt >= IRQ0) {
		ports_outb(PIC_PORT_MASTER, 0x20);
		if(cpu_state.interrupt == IRQ15) { return; }
		if(cpu_state.interrupt >= IRQ8) {
			ports_outb(PIC_PORT_SLAVE, 0x20);
		}
	}

	switch(cpu_state.interrupt) {
	case IRQ0:
		return;
	case IRQ1:
		ports_inb(0x60);
		break;
	}

	volatile unsigned char *ptr = (unsigned char *)0xB8000;
	volatile unsigned int *i = (unsigned int *)0x4000;
	ptr[0] = 'a' + (*i)++ % 26;

	char interrupt_string[3];
	uitoa(cpu_state.interrupt, interrupt_string, 16);
	ptr[80 * 2] = '0';
	ptr[81 * 2] = 'x';
	ptr[82 * 2] = interrupt_string[0];
	ptr[83 * 2] = interrupt_string[1];
	ptr[84 * 2] = interrupt_string[2];
}
