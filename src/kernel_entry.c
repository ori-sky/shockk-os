#include <stdint.h>
#include <kernel/memory.h>
#include <kernel/page_allocator.h>
#include <kernel/pager.h>
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/tss.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/syscall.h>
#include <kernel/pci.h>
#include <kernel/ata.h>

extern void user_enter(void *) __attribute__((noreturn));

void kernel_entry(struct Pager *pager) __attribute__((noreturn));
void kernel_entry(struct Pager *pager) {
	screen_init();

	pit_set(1 << 15);
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	struct IDT *idt = pager_reserve(pager);
	idt_init(idt);

	__asm__ ("sti");

	struct GDT *gdt = pager_reserve(pager);
	struct TSS *tss = pager_reserve(pager);
	gdt_init(gdt, tss);
	tss_init(tss);

	unsigned char *user_stack = pager_alloc(pager);
	user_enter(&user_stack[PAGE_ALLOCATOR_PAGE_SIZE]);
}

void user_main(void) __attribute__((noreturn));
void user_main(void) {
	syscall_put(':');
	syscall_put('D');
	syscall_put('\n');
	syscall_put('\n');
	for(;;);
}
