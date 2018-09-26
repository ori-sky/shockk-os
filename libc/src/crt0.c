#include <stdio.h>
#include <stdlib.h>

//extern void exit(int code);
extern int main();

void _start() {
	char *argv[] = {"/bin/hello.elf", 0};
	int ret = main(1, argv);
	exit(ret);
}
