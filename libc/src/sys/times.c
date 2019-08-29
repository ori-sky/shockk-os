#include <stdio.h>
#include <sys/times.h>

clock_t times(struct tms *buffer) {
	(void)buffer;
	puts("times: not implemented");
	return (clock_t)-1;
}
