#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H 1

int ioctl(int, unsigned long, ...);

#define TIOCGWINSZ 0x10000

struct winsize {
	unsigned short ws_row;
	unsigned short ws_col;
	unsigned short ws_xpixel; // unused
	unsigned short ws_ypixel; // unused
};

#endif
