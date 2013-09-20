#include "screen.h"

void main(void)
{
	screen_clear();
	screen_cursor_to(SCREEN_XYTOL(0, 4));
	screen_write('a');
	for(;;);
}
