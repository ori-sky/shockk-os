#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H 1

#include <stdint.h>

typedef uint32_t socklen_t;
typedef unsigned int sa_family_t;

struct sockaddr {
	sa_family_t sa_family;
	char sa_data[];
};

enum {
	SOCK_DGRAM,
	SOCK_RAW,
	SOCK_RDM,
	SOCK_SEQPACKET,
	SOCK_STREAM
};

enum {
	SOL_SOCKET
};

enum {
	SO_ACCEPTCONN,
	SO_BROADCAST,
	SO_DEBUG,
	SO_DONTROUTE,
	SO_ERROR,
	SO_KEEPALIVE,
	SO_LINGER,
	SO_OOBINLINE,
	SO_RCVBUF,
	SO_RCVLOWAT,
	SO_RCVTIMEO,
	SO_REUSEADDR,
	SO_SNDBUF,
	SO_SNDLOWAT,
	SO_SNDTIMEO,
	SO_TYPE
};

enum {
	AF_INET,
	AF_INET6,
	AF_UNIX,
	AF_UNSPEC
};

int getsockname(int, struct sockaddr * restrict, socklen_t * restrict);
int getpeername(int, struct sockaddr * restrict, socklen_t * restrict);

#endif
