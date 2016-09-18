#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/pager.h>
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/idt.h>

extern "C" void kernel_entry(Pager *) __attribute__((noreturn));
void kernel_entry(Pager *pager) {
	screen_init();

	pit_set(1 << 15); // programmable timer
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	IDT *idt = (IDT *)pager->Reserve();
	idt_init(idt);

	kernel_panic("call to kernel_entry succeeded!");
	for(;;) { __asm__ ("hlt"); }
}
