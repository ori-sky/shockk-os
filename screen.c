#include "screen.h"
#include "ports.h"

void screen_init(struct Screen *screen, volatile uint8_t *buffer) {
	screen->buffer = buffer;
	screen_cursor_to(screen, 0, 0);

	for(unsigned char row = 0; row < SCREEN_ROWS; ++row) {
		for(unsigned char col = 0; col < SCREEN_COLUMNS; ++col) {
			screen->buffer[SCREEN_COORDS_TO_CELL(row, col) << 1] = ' ';
			screen->buffer[(SCREEN_COORDS_TO_CELL(row, col) << 1) + 1] = ' ';
		}
	}
}

void screen_cursor_to(struct Screen *screen, uint8_t row, uint8_t column) {
	unsigned short cell = SCREEN_COORDS_TO_CELL(row, column);
	if(cell >= SCREEN_CELLS) cell = SCREEN_CELLS - 1;
	ports_outb(0x3D4, 14);
	ports_outb(0x3D5, (cell >> 8) & 0xFF);
	ports_outb(0x3D4, 15);
	ports_outb(0x3D5, cell & 0xFF);

	screen->row = row;
	screen->column = column;
}
