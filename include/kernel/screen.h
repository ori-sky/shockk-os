#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define SCREEN_ROWS 25
#define SCREEN_COLUMNS 80
#define SCREEN_CELLS (SCREEN_ROWS * SCREEN_COLUMNS)
#define SCREEN_BUFFER ((uint8_t *)0xB8000)

#define SCREEN_COORDS_TO_CELL(R, C) ((R) * SCREEN_COLUMNS + (C))
#define SCREEN_CELL_TO_ROW(C) ((C) / SCREEN_COLUMNS)

void screen_init(void);
void screen_cursor_to(const unsigned short);
void screen_cursor_by(short);
void screen_write_at(const unsigned short, const char);
char screen_read_at(const unsigned short);
void screen_put(const char);
void screen_print(const char *s);

#endif
