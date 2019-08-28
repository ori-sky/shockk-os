#ifndef _POLL_H
#define _POLL_H 1

struct pollfd {
	int fd;
	short events;
	short revents;
};

typedef unsigned int nfds_t;

#define POLLIN       0x1
#define POLLRDNORM   0x2
#define POLLRDBAND   0x4
#define POLLPRI      0x8
#define POLLOUT     0x10
#define POLLWRNORM  0x20
#define POLLWRBAND  0x40
#define POLLERR     0x80
#define POLLHUP    0x100
#define POLLNVAL   0x200

#endif
