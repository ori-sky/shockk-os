#include <poll.h>
#include <stdio.h>

int poll(struct pollfd fds[], nfds_t nfds, int timeout) {
	(void)fds;
	(void)nfds;
	(void)timeout;
	puts("poll: not implemented");
	return -1;
}
