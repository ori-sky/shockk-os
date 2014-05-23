#include <screen.h>

void screen_clear(void)
{
	unsigned char *vidmem = (unsigned char *)0xB8000;

	for(unsigned long i=0; i<SCREEN_SIZE; ++i)
	{
		*vidmem++ = 0;
		*vidmem++ = 0xF;
	}
}

void screen_putc(char c)
{

}
