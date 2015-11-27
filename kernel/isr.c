#include <kernel/ports.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/cpu.h>
#include <kernel/screen.h>
#include <kernel/itoa.h>

static unsigned int alpha_counter;

void isr_main(struct CPUState cpu_state) {
	if(cpu_state.interrupt == IRQ7) { return; }
	if(cpu_state.interrupt >= IRQ0) {
		ports_outb(PIC_PORT_MASTER, 0x20);
		if(cpu_state.interrupt == IRQ15) { return; }
		if(cpu_state.interrupt >= IRQ8) {
			ports_outb(PIC_PORT_SLAVE, 0x20);
		}
	}

	char gpf_s[] = "GPF at 0x           \n";
	char interrupt_s[] = "0x   \n";

	switch(cpu_state.interrupt) {
	case 0x0: /* divide by zero */
		screen_print("divide-by-zero error\n");
		break;
	case 0xD: /* general protection fault */
		itoa(cpu_state.iret_eip, &gpf_s[9], 16);
		screen_print(gpf_s);
		break;
	case IRQ0:
		break;
	default:
		ports_inb(0x60);
		screen_put('a' + alpha_counter++ % 26);
		itoa(cpu_state.interrupt, &interrupt_s[2], 16);
		screen_print(interrupt_s);
		break;
	}
}
