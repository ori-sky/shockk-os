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

extern "C" void isr_main(struct CPUState cpu_state, struct IRETState iret) {
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

	switch(cpu_state.interrupt) {
	case 0x0: /* divide by zero */
		_kernel_state.screen << "divide-by-zero at " << iret.eip << '\n';
		break;
	case 0xD: /* general protection fault */
		_kernel_state.screen << "GPF at " << iret.eip << " error=" << Screen::Binary(cpu_state.error) << '\n';
		break;
	case 0xE: /* page fault */
		_kernel_state.screen << "page fault at " << iret.eip << " cr2=" << cr2;
		_kernel_state.screen << " task=" << _kernel_state.task->pid;
		_kernel_state.screen << " error=" << Screen::Binary(cpu_state.error) << '\n';

		if(cr2 >= 0x1000000 && cr2 < 0x2000000) {
			_kernel_state.screen << "allocating page at " << cr2 << '\n';

			Pager::TableID table = cr2 / PAGE_ALLOCATOR_PAGE_SIZE / 1024;
			Pager::PageID  page  = cr2 / PAGE_ALLOCATOR_PAGE_SIZE % 1024;
			_kernel_state.task->context.AllocAt(table, page);
		} else {
			// try to print stack trace for page fault
			void **ebp = (void **)cpu_state.ebp;
			for(unsigned int frame = 0; frame < 4; ++frame) {
				// if ebp is outside of stack region
				//if(ebp < (void *)0x8000000 || ebp > (void *)(0x8000000 + 0x100000)) {
				//	_kernel_state.screen << "from <no frame pointer"\n";
				//	break;
				//}

				void * eip = ebp[1];
				if(eip == nullptr) { break; }
				ebp = (void **)ebp[0];

				_kernel_state.screen << "from " << eip << '\n';
			}
			kernel_panic("page fault");
		}
		break;
	case IRQ0: { /* PIT */
		auto curr = _kernel_state.task;
		if(curr != nullptr && curr->next != nullptr) {
			//_kernel_state.screen << "yield\n";
			task_switch(curr, curr->next);
		} else {
			_kernel_state.screen << "ran out of tasks\n";
		}
		break;
	}
	case IRQ1: { /* keyboard */
		uint8_t scancode = ports_inb(0x60); // read scancode from keyboard
		stdin_available = true;
		stdin_char = kb_scancode1_char(scancode);

		//_kernel_state.screen << "scancode = " << scancode << '\n';
		break;
	}
	case IRQ14: /* primary ATA channel */
		break;
	default:
		_kernel_state.screen << "unhandled interrupt: " << cpu_state.interrupt << '\n';
		break;
	}
}
