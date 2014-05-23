#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_X 80
#define SCREEN_Y 25
#define SCREEN_SIZE (SCREEN_X * SCREEN_Y)

extern void screen_clear(void);
extern void screen_putc(char);

#endif
