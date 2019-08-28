#include <malloc.h>
#include <stdio.h>

struct mallinfo mallinfo(void) {
	puts("mallinfo: not implemented");
	return (struct mallinfo){};
}

int mallopt(int param, int value) {
	(void)param;
	(void)value;
	puts("mallinfo: not implemented");
	return 0;
}
