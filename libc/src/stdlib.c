#include <stdio.h>
#include <stdlib.h>

void free(void *ptr) {
	
}

void * malloc(size_t size) {
	return NULL;
}

_Noreturn void exit(int status) {
	fprintf(stderr, "return code = %d\n", status);
	fflush(stdout);
	fflush(stderr);
	for(;;);
}
