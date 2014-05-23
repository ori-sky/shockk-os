#include <screen.h>

void entry(void)
{
	screen_clear();
	screen_cursor_to(0);
	screen_puts("ShockkOS v0.1.0");

	// loop
	for(;;);
}
