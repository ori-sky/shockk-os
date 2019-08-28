#ifndef _TERMIOS_H
#define _TERMIOS_H 1

typedef unsigned int cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

enum {
	VEOF,
	VEOL,
	VERASE,
	VINTR,
	VKILL,
	VMIN,
	VQUIT,
	VSTART,
	VSTOP,
	VSUSP,
	VTIME,
	NCCS
};

struct termios {
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
};

#define BRKINT   0x1
#define ICRNL    0x2
#define IGNBRK   0x4
#define IGNCR    0x8
#define IGNPAR  0x10
#define INLCR   0x20
#define INPCK   0x40
#define ISTRIP  0x80
#define IXANY  0x100
#define IXOFF  0x200
#define IXON   0x400
#define PARMRK 0x800

#define OPOST 0x1
#define ONLCR 0x2

enum {
	B0,
	B50,
	B75,
	B110,
	B134,
	B150,
	B200,
	B300,
	B600,
	B1200,
	B1800,
	B2400,
	B4800,
	B9600,
	B19200,
	B38400
};

#define ECHO     0x1
#define ECHOE    0x2
#define ECHOK    0x4
#define ECHONL   0x8
#define ICANON  0x10
#define IEXTEN  0x20
#define ISIG    0x40
#define NOFLSH  0x80
#define TOSTOP 0x100

enum {
	TCSANOW,
	TCSADRAIN,
	TCSAFLUSH
};

enum {
	TCIFLUSH,
	TCIOFLUSH,
	TCOFLUSH
};

#endif
