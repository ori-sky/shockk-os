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

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
	for(bool done = true; !done; done = true) {
		for(size_t i = 0; i < nmemb - 1; ++i) {
			int result = compar(base, base + size);
			if(result > 0) {
				unsigned char *s1 = base;
				unsigned char *s2 = base + size;
				for(size_t n = 0; n < size; ++n) {
					unsigned char tmp = s1[n];
					s1[n] = s2[n];
					s2[n] = tmp;
				}
			}
		}
	}
}
