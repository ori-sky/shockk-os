#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_X 80
#define SCREEN_Y 25
#define SCREEN_SIZE (SCREEN_X * SCREEN_Y)

#define SCREEN_XYTOL(X, Y) ((X) + (Y) * SCREEN_X)

unsigned short screen_get_cursor(void);
void screen_cursor_to(unsigned short);
void screen_writec_to(unsigned short, char);
void screen_writec(char);
void screen_putc(char);
void screen_puts(char *);
void screen_clear(void);

#endif
