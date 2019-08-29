#include <stdio.h>
#include <sys/time.h>

int gettimeofday(struct timeval *restrict tp, void *restrict tzp) {
	(void)tp;
	(void)tzp;
	puts("gettimeofday: not implemented");
	return 0;
}
