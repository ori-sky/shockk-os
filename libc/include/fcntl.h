#ifndef _FCNTL_H
#define _FCNTL_H 1

enum {
	F_DUPFD,
	F_GETFD,
	F_SETFD,
	F_GETFL,
	F_SETFL
};

enum {
	FD_CLOEXEC
};

enum {
	O_CREAT,
	O_EXCL,
	O_TRUNC,
	O_APPEND,
	O_NONBLOCK,
	O_RDONLY,
	O_RDWR,
	O_WRONLY
};

int open(const char *, int, ...);

#endif
