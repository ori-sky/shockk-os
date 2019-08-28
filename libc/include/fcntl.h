#ifndef _FCNTL_H
#define _FCNTL_H 1

enum {
	F_DUPFD,
	F_GETFD,
	F_SETFD,
	F_GETFL,
	F_SETFL
};

#define FD_CLOEXEC 1

#define O_CLOEXEC    0x1
#define O_CREAT      0x2
#define O_DIRECTORY  0x4
#define O_EXCL       0x8
#define O_NOCTTY    0x10
#define O_NOFOLLOW  0x20
#define O_TRUNC     0x40
#define O_TTY_INIT  0x80

#define O_APPEND    0x1
#define O_DSYNC     0x2
#define O_NONBLOCK  0x4
#define O_RSYNC     0x8
#define O_SYNC     0x10

#define O_EXEC   1
#define O_RDONLY 2
#define O_RDWR   3
#define O_SEARCH 4
#define O_WRONLY 5

int fcntl(int, int, ...);
int open(const char *, int, ...);

#endif
