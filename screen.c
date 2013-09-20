#include "screen.h"
#include "ports.h"

void screen_cursor_to(unsigned short location)
{
	ports_out(0x3D4, 14);
	ports_out(0x3D5, (location >> 8) & 0xFF);
	ports_out(0x3D4, 15);
	ports_out(0x3D5, location & 0xFF);
}

void screen_write_to(unsigned short location, unsigned char c)
{
	long offset = location * 2;
	unsigned char *vidmem = (unsigned char *)(0xB8000 + offset);
	*vidmem = c;
}

void screen_write(unsigned char c)
{
	unsigned short offset;

	ports_out(0x3D4, 14);
	offset = ports_in(0x3D5) << 8;
	ports_out(0x3D4, 15);
	offset |= ports_in(0x3D5);

	screen_write_to(offset, c);
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
