#include <errno.h>
#include <unistd.h>
#include <stdio.h>

char * getcwd(char *buf, size_t size) {
	if(size == 0) {
		errno = EINVAL;
		return NULL;
	}

	if(size < 1 + 1) {
		errno = ERANGE;
		return NULL;
	}

	buf[0] = '/';
	buf[1] = '\0';

	return buf;
}

long sysconf(int name) {
	(void)name;
	puts("sysconf: not implemented");
	return -1;
}
