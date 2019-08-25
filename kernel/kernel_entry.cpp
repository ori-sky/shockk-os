#include <kernel/elf.h>
#include <kernel/ext2.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/itoa.h>
#include <kernel/pager.h>
#include <kernel/panic.h>
#include <kernel/pic.h>
#include <kernel/pit.h>
#include <kernel/ports.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/syscall.h>
#include <kernel/task.h>
#include <kernel/tss.h>
#include <kernel/user.h>

extern "C" void kernel_entry(const State) __attribute__((noreturn));
void kernel_entry(const State state) {
	_kernel_state = state;

	_kernel_state.screen = Screen();
	_kernel_state.screen << "SHK\n";

	pit_set(1 << 15); // programmable timer
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	IDT *idt = (IDT *)_kernel_state.pager->GetContext().Reserve();
	idt_init(idt);

	__asm__ ("sti");

	GDT *gdt = (GDT *)_kernel_state.pager->GetContext().Reserve();
	TSS *tss = (TSS *)_kernel_state.pager->GetContext().Reserve();
	gdt_init(gdt, tss);

	_kernel_state.tss = tss;

	auto taskDash = Task::Create("/bin/dash.elf");

	taskDash->next = taskDash;

	tss_init(tss, taskDash->kernel_stack);
	task_switch(nullptr, taskDash);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
