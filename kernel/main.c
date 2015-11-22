#include <stdint.h>
#include <kernel/pic.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/tss.h>
#include <kernel/screen.h>
#include <kernel/syscall.h>

extern void user_enter(void);

void kernel_main(void) __attribute__((noreturn));
void kernel_main(void) {
	struct IDT *idt = (struct IDT *)0x500;
	struct TSS *tss = (struct TSS *)(0x500 + sizeof(struct IDT));
	struct GDT *gdt = (struct GDT *)(0x500 + sizeof(struct IDT) + sizeof(struct TSS));

	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);
	idt_init(idt);
	__asm__ __volatile__ ("sti");
	gdt_init(gdt, tss);
	tss_init(tss);
	screen_init();

	user_enter();
	for(;;) { __asm__ __volatile__ ("hlt"); }
}

void user_main(void) __attribute__((noreturn));
void user_main(void) {
	syscall_put(':');
	syscall_put('D');
	syscall_put('\n');
	for(;;);
}
