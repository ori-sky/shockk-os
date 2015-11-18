#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define SCREEN_ROWS 25
#define SCREEN_COLUMNS 80
#define SCREEN_CELLS (SCREEN_ROWS * SCREEN_COLUMNS)

#define SCREEN_COORDS_TO_CELL(R, C) ((R) * SCREEN_COLUMNS + (C))

struct Screen {
	volatile uint8_t *buffer;
	uint8_t row;
	uint8_t column;
} __attribute__((packed));

void screen_init(struct Screen *, volatile uint8_t *);
void screen_cursor_to(struct Screen *, uint8_t, uint8_t);

#endif
