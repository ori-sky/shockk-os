#include <interrupts.h>
#include <stdlib.h>
#include <kernel.h>
#include <ports.h>
#include <screen.h>

struct cpu_state * interrupts_callback(struct cpu_state *state)
{
	char str[4];
	uitoa(state->interrupt, str, 10);
	screen_puts("interrupt ");
	screen_puts(str);
	screen_putc('\n');

	switch(state->interrupt)
	{
		case EXC_DIVIDE_BY_ZERO:
			kernel_panic("divide by zero exception");
			break;
		case IRQ0:
			break;
		case IRQ1:
			ports_in(0x60);
			screen_putc(5 / 0);
			break;
	}
	return state;
}
