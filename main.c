#include <stdint.h>
#include "pic.h"
#include "irq.h"
#include "idt.h"
#include "gdt.h"
#include "tss.h"
#include "screen.h"

void entry(void) {
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

	__asm__ __volatile__ ("int $0x50");
	for(;;) { __asm__ __volatile__ ("hlt"); }
}
