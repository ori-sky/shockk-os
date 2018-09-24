#include <kernel/ports.h>
#include <kernel/screen.h>

static unsigned short screen_cell;

static void screen_scroll(uint8_t color) {
	for(unsigned char col = 0; col < SCREEN_COLUMNS; ++col) {
		for(unsigned char row = 0; row < SCREEN_ROWS - 1; ++row) {
			unsigned short old_cell = SCREEN_COORDS_TO_CELL(row + 1, col);
			screen_write_at(SCREEN_COORDS_TO_CELL(row, col), screen_read_at(old_cell), screen_color_at(old_cell));
		}
		screen_write_at(SCREEN_COORDS_TO_CELL(SCREEN_ROWS - 1, col), ' ', color);
	}
}

void screen_init(const uint8_t color) {
	for(unsigned short cell = 0; cell < SCREEN_CELLS; ++cell) {
		screen_write_at(cell, ' ', color);
	}
	screen_cursor_to(0);
}

void screen_cursor_to(const unsigned short cell) {
	ports_outb(0x3D4, 14);
	ports_outb(0x3D5, (cell >> 8) & 0xFF);
	ports_outb(0x3D4, 15);
	ports_outb(0x3D5, cell & 0xFF);

	screen_cell = cell;
}

void screen_cursor_by(short n, uint8_t color) {
	while(screen_cell + n >= SCREEN_CELLS) {
		screen_scroll(color);
		n -= SCREEN_COLUMNS;
	}
	screen_cursor_to((unsigned short)(screen_cell + n));
}

void screen_write_at(const unsigned short cell, const char c, const uint8_t color) {
	SCREEN_BUFFER[cell << 1] = (uint8_t)c;
	SCREEN_BUFFER[(cell << 1) + 1] = color;
}

char screen_read_at(const unsigned short cell) {
	return (char)SCREEN_BUFFER[cell << 1];
}

uint8_t screen_color_at(const unsigned short cell) {
	return SCREEN_BUFFER[(cell << 1) + 1];
}

void screen_put(const char c, const uint8_t color) {
	switch(c) {
	case '\0':
		break;
	case '\r':
		screen_cursor_to(SCREEN_COORDS_TO_CELL(SCREEN_CELL_TO_ROW(screen_cell), 0));
		break;
	case '\n':
		screen_cursor_to(SCREEN_COORDS_TO_CELL(SCREEN_CELL_TO_ROW(screen_cell), 0));
		screen_cursor_by(SCREEN_COLUMNS, color);
		break;
	default:
		screen_write_at(screen_cell, c, color);
		screen_cursor_by(1, color);
		break;
	}
}

void screen_print(const char *s, const uint8_t color) {
	while(*s != '\0') {
		screen_put(*s++, color);
	}
}
