#include <stdint.h>
#include "ports.h"
#include "pic.h"
#include "idt.h"
#include "irq.h"
#include "screen.h"

void entry(void) {
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0, 0);

	idt_init((struct IDT *)0x500);
	__asm__ __volatile__ ("sti");

	screen_init();

	__asm__ __volatile__ ("int $0x50");
	for(;;) { __asm__ __volatile__ ("hlt"); }
}
