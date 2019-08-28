#ifndef _NETDB_H
#define _NETDB_H 1

#include <sys/socket.h>

struct hostent {
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_length;
	char **h_addr_list;
};

struct servent {
	char *s_name;
	char **s_aliases;
	int s_port;
	char *s_proto;
};

struct addrinfo {
	int ai_flags;
	int ai_family;
	int ai_socktype;
	int ai_protocol;
	socklen_t ai_addrlen;
	struct sockaddr *ai_addr;
	char *ai_canonname;
	struct addrinfo *ai_next;
};

#define AI_PASSIVE      0x1
#define AI_CANONNAME    0x2
#define AI_NUMERICHOST  0x4
#define AI_NUMERICSERV  0x8
#define AI_V4MAPPED    0x10
#define AI_ALL         0x20
#define AI_ADDRCONFIG  0x40

#define NI_NOFQDN        0x1
#define NI_NUMERICHOST   0x2
#define NI_NAMEREQD      0x4
#define NI_NUMERICSERV   0x8
#define NI_NUMERICSCOPE 0x10
#define NI_DGRAM        0x20

#endif
