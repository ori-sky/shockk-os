#include "screen.h"

void main(void)
{
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
