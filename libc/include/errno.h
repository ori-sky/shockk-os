#ifndef _ERRNO_H
#define _ERRNO_H 1

enum _errno_e {
	EACCES,
	EAGAIN,
	EBADF,
	EEXIST,
	EINTR,
	ENOENT,
	ENOEXEC,
	ENOTDIR,
	ERANGE,
	EWOULDBLOCK
};

int errno;

#endif
