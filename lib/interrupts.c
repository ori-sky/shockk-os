#include <interrupts.h>
#include <stdlib.h>
#include <kernel.h>
#include <ports.h>
#include <screen.h>

struct cpu_state * interrupts_callback(struct cpu_state *state)
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
			screen_puts("key ");
			screen_puts(scancode & 0x80 ? "up" : "down");
			screen_puts(": ");
			screen_puts(uitoa(scancode & 0x7F, str, 16));
			screen_putc('\n');
			break;
	}
	return state;
}
