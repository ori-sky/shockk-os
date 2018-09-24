#ifndef SCREEN_H
#define SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SCREEN_ROWS 25
#define SCREEN_COLUMNS 80
#define SCREEN_CELLS (SCREEN_ROWS * SCREEN_COLUMNS)
#define SCREEN_BUFFER ((uint8_t *)0xB8000)

#define SCREEN_COORDS_TO_CELL(R, C) ((R) * SCREEN_COLUMNS + (C))
#define SCREEN_CELL_TO_ROW(C) ((C) / SCREEN_COLUMNS)

#define SCREEN_COLOR_DEFAULT 0x20
#define SCREEN_COLOR_USER    0x30

void screen_init(const uint8_t = SCREEN_COLOR_DEFAULT);
void screen_cursor_to(const unsigned short);
void screen_cursor_by(short, const uint8_t = SCREEN_COLOR_DEFAULT);
void screen_write_at(const unsigned short, const char, const uint8_t = SCREEN_COLOR_DEFAULT);
char screen_read_at(const unsigned short);
uint8_t screen_color_at(const unsigned short);
void screen_put(const char, const uint8_t = SCREEN_COLOR_DEFAULT);
void screen_print(const char *s, const uint8_t = SCREEN_COLOR_DEFAULT);

#ifdef __cplusplus
}
#endif

#endif
