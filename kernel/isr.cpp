#include <kernel/ports.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/cpu.h>
#include <kernel/screen.h>
#include <kernel/panic.h>
#include <kernel/itoa.h>

bool stdin_available = false;
char stdin_char = '\0';

static unsigned int alpha_counter;

extern "C" void isr_main(struct CPUState cpu_state) {
	if(cpu_state.interrupt == IRQ7) { return; }
	if(cpu_state.interrupt >= IRQ0) {
		ports_outb(PIC_PORT_MASTER, 0x20);
		if(cpu_state.interrupt == IRQ15) { return; }
		if(cpu_state.interrupt >= IRQ8) {
			ports_outb(PIC_PORT_SLAVE, 0x20);
		}
	}

	uint32_t cr2;
	__asm__ __volatile__ ("mov %%cr2, %0" : "=r" (cr2));

	char s0[] = "        ";
	char s1[] = "                                ";

	switch(cpu_state.interrupt) {
	case 0x0: /* divide by zero */
		screen_print("divide-by-zero error\n");
		break;
	case 0xD: /* general protection fault */
		screen_print("GPF at 0x");
		uitoa((unsigned int)cpu_state.iret_eip, s0, 16);
		screen_print(s0);
		screen_print(" error=0b");
		uitoa((unsigned int)cpu_state.error, s1, 2);
		screen_print(s1);
		screen_put('\n');
		break;
	case 0xE: /* page fault */
		screen_print("page fault at 0x");
		uitoa((unsigned int)cpu_state.iret_eip, s0, 16);
		screen_print(s0);
		screen_print(" cr2=");
		uitoa((unsigned int)cr2, s0, 16);
		screen_print(s0);
		screen_print(" error=0b");
		uitoa((unsigned int)cpu_state.error, s1, 2);
		screen_print(s1);
		screen_put('\n');
		{
			void **ebp = (void **)cpu_state.ebp;
			for(unsigned int frame = 0; frame < 8; ++frame) {
				void * eip = ebp[1];
				if(eip == NULL) { break; }
				ebp = (void **)ebp[0];
				screen_print("from 0x");
				s0[uitoa((unsigned int)eip, s0, 16)] = '\0';
				screen_print(s0);
				screen_put('\n');
			}
		}
		kernel_panic("");
		break;
	case IRQ0: /* PIT */
		break;
	case IRQ14: /* primary ATA channel */
		break;
	default:
		ports_inb(0x60);
		screen_put('a' + alpha_counter++ % 26);
		screen_print("0x");
		uitoa((unsigned int)cpu_state.interrupt, s0, 16);
		screen_print(s0);
		screen_put('\n');
		break;
	}
}
