#include "screen.h"
#include "ports.h"

void screen_cursor_to(unsigned short location)
{
	ports_out(0x3D4, 14);
	ports_out(0x3D5, (location >> 8) & 0xFF);
	ports_out(0x3D4, 15);
	ports_out(0x3D5, location & 0xFF);
}

void screen_clear(void)
{
	unsigned char *vidmem = (unsigned char *)0xB8000;

	for(long loop=0; loop<SCREEN_SIZE; ++loop)
	{
		*vidmem++ = 0;
		*vidmem++ = 0xF;
	}
}
