#ifndef _SYS_UN_H
#define _SYS_UN_H 1

#include <sys/socket.h>

#define MAX_SOCKADDR_UN_PATH 128 

struct sockaddr_un {
	sa_family_t sun_family;
	char sun_path[MAX_SOCKADDR_UN_PATH];
};

#endif
