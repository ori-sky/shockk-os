#ifndef _NETINET_IN_H
#define _NETINET_IN_H 1

#include <inttypes.h>
#include <sys/socket.h>

typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

struct in_addr {
	in_addr_t s_addr;
};

struct sockaddr_in {
	sa_family_t sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
};

enum {
	INADDR_ANY
};

#endif
