#include <stdint.h>
#include <kernel/panic.h>

#define SCREEN_BUFFER ((uint8_t *)0xB8000)

void kernel_panic(const char *s) {
	(void)s;

	for(unsigned short cell = 0; cell < 80 * 25; ++cell) {
		if(cell >= 80 && *s != '\0') {
			SCREEN_BUFFER[cell << 1] = (unsigned char)*s++;
		} else {
			SCREEN_BUFFER[cell << 1] = ' ';
		}
		SCREEN_BUFFER[(cell << 1) + 1] = ' ';
	}

	SCREEN_BUFFER[0] = 'P';
	SCREEN_BUFFER[2] = 'A';
	SCREEN_BUFFER[4] = 'N';
	SCREEN_BUFFER[6] = 'I';
	SCREEN_BUFFER[8] = 'C';

	for(;;) { __asm__ ("hlt"); }
}
