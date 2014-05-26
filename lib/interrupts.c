#include <interrupts.h>
#include <kernel.h>
#include <ports.h>
#include <screen.h>

struct cpu_state * interrupts_callback(struct cpu_state *state)
{
	char interrupt[4];
	interrupt[0] = 0x30 + state->interrupt / 100 % 10;
	interrupt[1] = 0x30 + state->interrupt / 10 % 10;
	interrupt[2] = 0x30 + state->interrupt % 10;
	interrupt[3] = 0;
	//screen_puts("interrupt ");
	//screen_puts(interrupt);
	//screen_putc('\n');

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
