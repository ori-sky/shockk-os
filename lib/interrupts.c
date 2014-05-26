#include <interrupts.h>
#include <ports.h>
#include <screen.h>

struct cpu_state * interrupts_callback(struct cpu_state *state)
{
	static char counter = 0;
	screen_putc(counter++);
	screen_putc(' ');

	char interrupt[4];
	interrupt[0] = 0x30 + state->interrupt / 100 % 10;
	interrupt[1] = 0x30 + state->interrupt / 10 % 10;
	interrupt[2] = 0x30 + state->interrupt % 10;
	interrupt[3] = 0;
	screen_puts("interrupt ");
	screen_puts(interrupt);
	//screen_putc('\n');

	switch(state->interrupt)
	{
		case IRQ0:
			break;
		case IRQ1:
			ports_in(0x60);
			break;
	}
	return state;
}
