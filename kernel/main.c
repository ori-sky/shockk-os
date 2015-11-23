#include <stdint.h>
#include <kernel/a20.h>
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/tss.h>
#include <kernel/screen.h>
#include <kernel/syscall.h>
#include <kernel/panic.h>

extern void user_enter(void);

void kernel_main(void) __attribute__((noreturn));
void kernel_main(void) {
	screen_init();

	if(a20_enable()) { kernel_panic("failed to enable A20 line"); }

	struct IDT *idt = (struct IDT *)0x500;
	struct TSS *tss = (struct TSS *)(0x500 + sizeof(struct IDT));
	struct GDT *gdt = (struct GDT *)(0x500 + sizeof(struct IDT) + sizeof(struct TSS));

	pit_set(1 << 15);
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);
	idt_init(idt);

	__asm__ ("sti");

	gdt_init(gdt, tss);
	tss_init(tss);

	user_enter();
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
