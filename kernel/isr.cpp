#include <kernel/cpu.h>
#include <kernel/irq.h>
#include <kernel/itoa.h>
#include <kernel/kb.h>
#include <kernel/panic.h>
#include <kernel/pic.h>
#include <kernel/ports.h>
#include <kernel/screen.h>
#include <kernel/state.h>

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
	char s1[] = "                ";
	char s2[] = "                                ";
	uint8_t scancode;

	switch(cpu_state.interrupt) {
	case 0x0: /* divide by zero */
		screen_print("divide-by-zero error\n");
		break;
	case 0xD: /* general protection fault */
		screen_print("GPF at 0x");
		uitoa((unsigned int)cpu_state.iret_eip, s0, 16);
		screen_print(s0);
		screen_print(" error=0b");
		uitoa((unsigned int)cpu_state.error, s2, 2);
		screen_print(s2);
		screen_put('\n');
		break;
	case 0xE: /* page fault */
		screen_print("page fault at 0x");
		uitoa((unsigned int)cpu_state.iret_eip, s0, 16);
		screen_print(s0);
		screen_print(" cr2=0x");
		uitoa((unsigned int)cr2, s0, 16);
		screen_print(s0);
		screen_print(" error=0b");
		uitoa((unsigned int)cpu_state.error, s1, 2);
		screen_print(s1);
		screen_put('\n');
		if(cr2 >= 0x1000000 && cr2 < 0x2000000) {
			screen_print("allocating page at 0x");
			uitoa((unsigned int)cr2, s0, 16);
			screen_print(s0);
			screen_put('\n');

			Pager::TableID table = cr2 / PAGE_ALLOCATOR_PAGE_SIZE / 1024;
			Pager::PageID  page  = cr2 / PAGE_ALLOCATOR_PAGE_SIZE % 1024;
			_kernel_state.pager->GetContext().AllocAt(table, page);
		} else {
			// try to print stack trace for page fault
			void **ebp = (void **)cpu_state.ebp;
			for(unsigned int frame = 0; frame < 3; ++frame) {
				// if ebp is outside of stack region
				if(ebp < (void *)0x8000000 || ebp > (void *)(0x8000000 + 0x100000)) {
					screen_print("from <no frame pointer>\n");
					break;
				}

				void * eip = ebp[1];
				if(eip == NULL) { break; }
				ebp = (void **)ebp[0];

				screen_print("from 0x");
				s0[uitoa((unsigned int)eip, s0, 16)] = '\0';
				screen_print(s0);
				screen_put('\n');
			}
			kernel_panic("page fault");
		}
		break;
	case IRQ0: /* PIT */
		if(_kernel_state.next != nullptr) {
			screen_print("yield\n");
			//task_switch(next);
			_kernel_state.next = nullptr;
		}
		break;
	case IRQ1: /* keyboard */
		scancode = ports_inb(0x60); // read scancode from keyboard
		stdin_available = true;
		//stdin_char = 'a' + alpha_counter++ % 26;
		stdin_char = kb_scancode1_char(scancode);

		//screen_print("scancode = 0x");
		//uitoa((unsigned int)scancode, s0, 16);
		//screen_print(s0);
		//screen_put('\n');
		break;
	case IRQ14: /* primary ATA channel */
		break;
	default:
		screen_print("unhandled interrupt: 0x");
		uitoa((unsigned int)cpu_state.interrupt, s0, 16);
		screen_print(s0);
		screen_put('\n');
		break;
	}
}
