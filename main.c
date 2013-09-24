#include "screen.h"
#include "pic.h"

void kernel_entry(void)
{
	pic_remap(0x20, 0x28);
	screen_clear();
	screen_cursor_to(SCREEN_XYTOL(0, 4));

	screen_putc('h');
	screen_putc('e');
	screen_putc('l');
	screen_putc('l');
	screen_putc('o');

	screen_puts(" world");

	// loop
	for(;;);
}

void isr_entry(void)
{
	screen_putc('0');
}
