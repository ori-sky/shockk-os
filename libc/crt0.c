#include <fcntl.h>
#include <stdio.h>

//extern void exit(int code);
extern int main();

void _start() {
	int ret = main();
	fprintf(stderr, "return code = %d\n", ret);
	fflush(stdout);
	fflush(stderr);
	for(;;);
}
