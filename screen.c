#include "screen.h"
#include "ports.h"

unsigned short screen_get_cursor(void)
{
	unsigned short location;

	ports_out(0x3D4, 14);
	location = ports_in(0x3D5) << 8;
	ports_out(0x3D4, 15);
	location |= ports_in(0x3D5);

	return location;
}

void screen_cursor_to(unsigned short location)
{
	ports_out(0x3D4, 14);
	ports_out(0x3D5, (location >> 8) & 0xFF);
	ports_out(0x3D4, 15);
	ports_out(0x3D5, location & 0xFF);
}

void screen_writec_to(unsigned short location, unsigned char c)
{
	long offset = location * 2;
	unsigned char *vidmem = (unsigned char *)(0xB8000 + offset);
	*vidmem = c;
}

void screen_writec(unsigned char c)
{
	screen_writec_to(screen_get_cursor(), c);
}

void screen_putc(unsigned char c)
{
	screen_writec(c);
	// TODO: prevent framebuffer overflows
	screen_cursor_to(screen_get_cursor() + 1);
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
