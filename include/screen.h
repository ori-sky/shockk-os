#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_X 80
#define SCREEN_Y 25
#define SCREEN_SIZE (SCREEN_X * SCREEN_Y)

#define SCREEN_XYTOLOC(X, Y) ((X) + (Y) * SCREEN_X)
#define SCREEN_LOCTOX(L) ((L) % SCREEN_X)
#define SCREEN_LOCTOY(L) ((L) / SCREEN_X)

extern unsigned short screen_cursor_loc(void);
extern void screen_cursor_to(unsigned short);
extern void screen_cursor_by(unsigned short);
extern void screen_clear(void);
extern void screen_writec(char, unsigned short);
extern void screen_putc(char);
extern void screen_puts(char *);

#endif
