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

	char s0[] = "        ";
	char s1[] = "                                ";

	switch(cpu_state.interrupt) {
	case 0x0: /* divide by zero */
		screen_print("divide-by-zero error\n");
		break;
	case 0xD: /* general protection fault */
		screen_print("GPF at 0x");
		itoa((int)cpu_state.iret_eip, s0, 16);
		screen_print(s0);
		screen_print(" error=0b");
		itoa((int)cpu_state.error, s1, 2);
		screen_print(s1);
		screen_put('\n');
		break;
	case 0xE: /* page fault */
		screen_print("page fault at 0x");
		itoa((int)cpu_state.iret_eip, s0, 16);
		screen_print(s0);
		screen_print(" error=0b");
		itoa((int)cpu_state.error, s1, 2);
		screen_print(s1);
		screen_put('\n');
		break;
	case IRQ0: /* PIT */
		break;
	case IRQ14: /* primary ATA channel */
		break;
	default:
		ports_inb(0x60);
		screen_put('a' + alpha_counter++ % 26);
		screen_print("0x");
		itoa((int)cpu_state.interrupt, s0, 16);
		screen_print(s0);
		screen_put('\n');
		break;
	}
}
