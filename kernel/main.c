#include <stdint.h>
#include <kernel/a20.h>
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/tss.h>
#include <kernel/panic.h>
#include <kernel/memory.h>
#include <kernel/screen.h>
#include <kernel/syscall.h>

extern void user_enter(void *);

void kernel_main(void) __attribute__((noreturn));
void kernel_main(void) {
	volatile struct IDT *idt = kmalloc(sizeof(struct IDT));
	volatile struct TSS *tss = kmalloc(sizeof(struct TSS));
	volatile struct GDT *gdt = kmalloc(sizeof(struct GDT));

	screen_init();

	if(!a20_enable()) { kernel_panic("failed to enable A20 line"); }

	pit_set(1 << 15);
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);
	idt_init(idt);

	__asm__ ("sti");

	gdt_init(gdt, tss);
	tss_init(tss);

	user_enter(kmalloc(1 << 20));
	for(;;) { __asm__ ("hlt"); }
}

void user_main(void) __attribute__((noreturn));
void user_main(void) {
	syscall_put(':');
	syscall_put('D');
	syscall_put('\n');
	syscall_put('\n');
	for(;;);
}
