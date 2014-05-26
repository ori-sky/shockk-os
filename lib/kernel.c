#include <kernel.h>
#include <gdt.h>
#include <pic.h>
#include <idt.h>
#include <interrupts.h>
#include <screen.h>

void kernel_main(void)
{
	gdt_init();
	pic_remap(IRQ0, IRQ8);
	pic_set_masks(0xFC, 0xFF);
	idt_init();

	screen_clear();
	screen_cursor_to(0);
	screen_puts("Shockk OS v0.1.0\n");

	__asm__ volatile("sti");

	/* loop */
	for(;;)
		__asm__("hlt");
}

void kernel_panic(char *str)
{
	screen_puts("\nKERNEL PANIC\n");
	screen_puts(str);

	__asm__ volatile("cli");
	__asm__("hlt");
}
