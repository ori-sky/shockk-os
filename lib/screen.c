#include <screen.h>
#include <ports.h>

unsigned short screen_cursor_loc(void)
{
	unsigned short location;

	ports_out(0x3D4, 14);
	location = ports_in(0x3D5) << 8;
	ports_out(0x3D4, 15);
	location |= ports_in(0x3D5);

	return location;
}

void screen_cursor_to(unsigned short loc)
{
	ports_out(0x3D4, 14);
	ports_out(0x3D5, (loc >> 8) & 0xFF);
	ports_out(0x3D4, 15);
	ports_out(0x3D5, loc & 0xFF);
}

void screen_cursor_by(unsigned short n)
{
	screen_cursor_to(screen_cursor_loc() + n);
}

void screen_clear(void)
{
	unsigned char *vidmem = (unsigned char *)0xB8000;

	for(unsigned short i=0; i<SCREEN_SIZE; ++i)
	{
		*vidmem++ = 0;
		*vidmem++ = 0xF;
	}
}

void screen_writec(char c, unsigned short loc)
{
	unsigned long offset = loc * 2;
	unsigned char *vidmem = (unsigned char *)(0xB8000 + offset);
	*vidmem = c;
}

void screen_putc(char c)
{
	screen_writec(c, screen_cursor_loc());
	screen_cursor_by(1);
}

void screen_puts(char *s)
{
	for(; *s!=0; ++s)
	{
		screen_putc(*s);
	}
}
