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

char screen_readc(unsigned short loc)
{
	unsigned long offset = loc * 2;
	unsigned char *vidmem = (unsigned char *)(0xB8000 + offset);
	return *vidmem;
}

void screen_writec(char c, unsigned short loc)
{
	unsigned long offset = loc * 2;
	unsigned char *vidmem = (unsigned char *)(0xB8000 + offset);
	*vidmem = c;
}

void screen_scroll(void)
{
	for(unsigned short x=0; x<SCREEN_X; ++x)
	{
		for(unsigned short y=0; y<SCREEN_Y-1; ++y)
		{
			screen_writec(screen_readc(SCREEN_XYTOLOC(x, y + 1)), SCREEN_XYTOLOC(x, y));
		}

		screen_writec(0, SCREEN_XYTOLOC(x, SCREEN_Y - 1));
	}
}

void screen_putc(char c)
{
	unsigned short loc = screen_cursor_loc();
	unsigned short newloc;
	switch(c)
	{
		case '\n':
			if(SCREEN_LOCTOY(loc) == SCREEN_Y - 1)
			{
				screen_scroll();
				newloc = SCREEN_XYTOLOC(0, SCREEN_LOCTOY(loc));
			}
			else newloc = SCREEN_XYTOLOC(0, SCREEN_LOCTOY(loc) + 1);
			screen_cursor_to(newloc);
			break;
		default:
			screen_writec(c, screen_cursor_loc());
			if(loc == SCREEN_SIZE - 1)
			{
				screen_scroll();
				screen_cursor_to(SCREEN_XYTOLOC(0, SCREEN_Y - 1));
			}
			else screen_cursor_by(1);
			break;
	}
}

void screen_puts(char *s)
{
	for(; *s!=0; ++s)
	{
		screen_putc(*s);
	}
}
