#include <kernel/ports.h>
#include <kernel/pit.h>

void pit_set(uint16_t divisor) {
	__asm__ ("pushf"); /* push current interrupts state */
	__asm__ ("cli"); /* disable interrupts temporarily */

	/* 00 = channel 0
	 * 11 = access mode: low byte / high byte
	 * 010 = operating mode: rate generator
	 * 0 = 16-bit binary
	 * 0b00110100 == 0x34
	 */
	ports_outb(0x43, 0x34);
	ports_outb(0x40, divisor & 0xFF);
	ports_outb(0x40, (divisor >> 8) & 0xFF);

	__asm__ ("popf"); /* pop previous interrupts state */
}
