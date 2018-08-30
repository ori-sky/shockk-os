#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/pager.h>
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/tss.h>
#include <kernel/syscall.h>

extern "C" void user_enter(void *) __attribute__((noreturn));

extern "C" void kernel_entry(Pager *) __attribute__((noreturn));
void kernel_entry(Pager *pager) {
	screen_init();
	screen_put('S');
	screen_put('H');
	screen_put('K');
	screen_put('\n');
	//screen_print("SHK\n");

	kernel_panic("in kernel mode (1)");

	pit_set(1 << 15); // programmable timer
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	IDT *idt = (IDT *)pager->Reserve();
	idt_init(idt);

	__asm__ ("sti");

	GDT *gdt = (GDT *)pager->Reserve();
	TSS *tss = (TSS *)pager->Reserve();
	gdt_init(gdt, tss);
	tss_init(tss, pager);

	kernel_panic("in kernel mode");

	unsigned char *user_stack = (unsigned char *)pager->Alloc();
	user_enter(&user_stack[PAGE_ALLOCATOR_PAGE_SIZE]);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
