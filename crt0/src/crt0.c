#include <stdio.h>
#include <stdlib.h>

//extern void exit(int code);
extern int main();

_Noreturn void _start(int argc, char *argv[]) {
	//for(size_t i = 0; argv[i] != NULL; ++i) {
	//	printf("argv[%u] = %s\n", (unsigned int)i, argv[i]);
	//}
	int ret = main(argc, argv);
	exit(ret);
}
