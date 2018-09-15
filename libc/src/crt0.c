#include <stdio.h>
#include <stdlib.h>

//extern void exit(int code);
extern int main();

void _start() {
	int ret = main();
	exit(ret);
}
