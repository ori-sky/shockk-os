#include <gdt.h>
#include <pic.h>
#include <screen.h>

void entry(void)
{
	gdt_init();
	pic_remap(0x20, 0x28);

	screen_clear();
	screen_cursor_to(0);
	screen_puts("ShockkOS v0.1.0\n");

	// loop
	for(;;);
}
