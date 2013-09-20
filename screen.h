#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_X 80
#define SCREEN_Y 25
#define SCREEN_SIZE (SCREEN_X * SCREEN_Y)

#define SCREEN_XYTOL(X, Y) ((X) + (Y) * SCREEN_X)

unsigned short screen_get_cursor(void);
void screen_cursor_to(unsigned short);
void screen_writec_to(unsigned short, unsigned char);
void screen_writec(unsigned char);
void screen_putc(unsigned char);
void screen_puts(unsigned char *);
void screen_clear(void);

#endif
