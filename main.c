#include <gdt.h>
#include <pic.h>
#include <idt.h>
#include <screen.h>

void entry(void)
{
	gdt_init();
	pic_remap(0x20, 0x28);
	idt_init();

	screen_clear();
	screen_cursor_to(0);
	screen_puts("ShockkOS v0.1.0\n");

	__asm__ volatile("sti");

	// loop
	for(;;)
	{
		__asm__("hlt");
	}
}
