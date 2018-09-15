#include <stdio.h>
#include <stdlib.h>

_Noreturn void exit(int status) {
	fprintf(stderr, "return code = %d\n", status);
	fflush(stdout);
	fflush(stderr);
	for(;;);
}
