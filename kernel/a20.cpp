#include <kernel/ports.h>
#include <arch/x86/a20.h>

static void a20_wait_test(unsigned char test) {
	while(ports_inb(0x64) & test) {}
}

static void a20_outb(unsigned short port, uint8_t byte) {
	a20_wait_test(2);
	ports_outb(port, byte);
}

void a20_enable_kbd(void) {
	a20_outb(0x64, 0xAD); /* disable keyboard */
	a20_outb(0x64, 0xD0); /* read from input */

	a20_wait_test(1);
	unsigned char input = ports_inb(0x60); /* read data */

	a20_outb(0x64, 0xD1); /* write to output */
	a20_outb(0x60, input | 2); /* send data */
	a20_outb(0x63, 0xAE);
}

void a20_enable_fast(void) {
	unsigned char input = ports_inb(0x92);
	a20_outb(0x92, input | 2);
}

bool a20_test(void) {
	/* test if bootsector signature does not wrap around past 1 MB */
	return *(unsigned short *)0x7DFE != *(unsigned short *)(0x7DFE + (1 << 20));
}

bool a20_enable(void) {
	if(a20_test()) { return true; }
	a20_enable_kbd();
	if(a20_test()) { return true; }
	a20_enable_fast();
	return a20_test();
}
