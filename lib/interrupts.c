#include <interrupts.h>
#include <stdlib.h>
#include <kernel.h>
#include <ports.h>
#include <screen.h>

struct cpu_state * interrupts_callback(struct cpu_state *state)
{
	char str[4];

	switch(state->interrupt)
	{
		case EXC_DIVIDE_BY_ZERO:
			kernel_panic("divide by zero exception");
			break;
		case IRQ0:
			break;
		case IRQ1:
			uitoa(ports_in(0x60), str, 16);
			screen_puts("scancode 0x");
			screen_puts(str);
			screen_putc('\n');
			break;
	}
	return state;
}
