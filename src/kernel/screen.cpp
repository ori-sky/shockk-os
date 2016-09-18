#include <kernel/ports.h>
#include <kernel/screen.h>

static unsigned short screen_cell;

static void screen_scroll(void) {
	for(unsigned char col = 0; col < SCREEN_COLUMNS; ++col) {
		for(unsigned char row = 0; row < SCREEN_ROWS - 1; ++row) {
			screen_write_at(SCREEN_COORDS_TO_CELL(row, col), screen_read_at(SCREEN_COORDS_TO_CELL(row + 1, col)));
		}
		screen_write_at(SCREEN_COORDS_TO_CELL(SCREEN_ROWS - 1, col), ' ');
	}
}

void screen_init(void) {
	for(unsigned short cell = 0; cell < SCREEN_CELLS; ++cell) {
		screen_write_at(cell, ' ');
		SCREEN_BUFFER[(cell << 1) + 1] = ' ';
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

void screen_cursor_by(short n) {
	while(screen_cell + n >= SCREEN_CELLS) {
		screen_scroll();
		n -= SCREEN_COLUMNS;
	}
	screen_cursor_to((unsigned short)(screen_cell + n));
}

void screen_write_at(const unsigned short cell, const char c) {
	SCREEN_BUFFER[cell << 1] = (uint8_t)c;
}

char screen_read_at(const unsigned short cell) {
	return (char)SCREEN_BUFFER[cell << 1];
}

void screen_put(const char c) {
	switch(c) {
	case '\0':
		break;
	case '\r':
		screen_cursor_to(SCREEN_COORDS_TO_CELL(SCREEN_CELL_TO_ROW(screen_cell), 0));
		break;
	case '\n':
		screen_cursor_to(SCREEN_COORDS_TO_CELL(SCREEN_CELL_TO_ROW(screen_cell), 0));
		screen_cursor_by(SCREEN_COLUMNS);
		break;
	default:
		screen_write_at(screen_cell, c);
		screen_cursor_by(1);
		break;
	}
}

void screen_print(const char *s) {
	while(*s != '\0') {
		screen_put(*s++);
	}
}
