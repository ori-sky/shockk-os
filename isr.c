#include "ports.h"
#include "pic.h"
#include "irq.h"
#include "cpu.h"
#include "screen.h"

static unsigned int i;

inline char * uitoa(unsigned int value, char *str, unsigned int base) {
	unsigned char log = 0;
	for(unsigned int n=value; n>=base; n/=base) { ++log; }

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
	case IRQ1:
		ports_inb(0x60);
		break;
	}

	screen_put('a' + i++ % 26);

	char interrupt_string[] = "0x00\n";
	uitoa(cpu_state.interrupt, &interrupt_string[2], 16);
	screen_print(interrupt_string);
}
