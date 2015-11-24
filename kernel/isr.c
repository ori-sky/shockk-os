#include <kernel/ports.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/cpu.h>
#include <kernel/screen.h>

static unsigned int i;

char * uitoa(unsigned int value, char *str, unsigned int base) {
	unsigned char log = 0;
	for(unsigned int n=value; n>=base; n/=base) { ++log; }

	unsigned short div = 1;
	for(unsigned char i=log; i!=(unsigned char)(-1); --i, div*=base) {
		unsigned char offset = value / div % base;
		unsigned char base = offset < 10 ? '0' : 'A' - 10;
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

	char interrupt_string[] = "0x  \n";

	switch(cpu_state.interrupt) {
	case IRQ1:
		ports_inb(0x60);
		screen_put('a' + i++ % 26);
		uitoa(cpu_state.interrupt, &interrupt_string[2], 16);
		screen_print(interrupt_string);
		break;
	}
}
