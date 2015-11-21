#include <stdint.h>
#include "ports.h"
#include "pic.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "screen.h"

void entry(void) {
	struct IDT *idt = (struct IDT *)0x500;
	struct GDT *gdt = (struct GDT *)(0x500 + sizeof(struct GDT));

	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);
	idt_init(idt);
	__asm__ __volatile__ ("sti");
	gdt_init(gdt);
	screen_init();

	__asm__ __volatile__ ("int $0x50");
	for(;;) { __asm__ __volatile__ ("hlt"); }
}
