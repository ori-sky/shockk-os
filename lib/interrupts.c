#include <interrupts.h>
#include <stdlib.h>
#include <kernel.h>
#include <ports.h>
#include <keyboard.h>
#include <screen.h>

struct cpu_state * interrupts_handler(struct cpu_state *state)
{
	char str[32];
	unsigned char scancode;

	switch(state->interrupt)
	{
		case EXC_DIVIDE_BY_ZERO:
			kernel_panic("divide by zero exception");
			break;
		case IRQ0:
			break;
		case IRQ1:
			scancode = ports_inb(0x60);
			if(!(scancode & 0x80)) screen_putc(keymap_en[scancode & 0x7F]);
			break;
		case 0x80: /* syscall */
			break;
	}
	return state;
}
