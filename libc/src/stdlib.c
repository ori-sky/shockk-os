#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

long long int strtoll(const char * restrict nptr, char ** restrict endptr, int base) {
	(void)nptr;
	(void)endptr;
	(void)base;
	puts("strtoll: not implemented");
	return 0;
}

unsigned long long int strtoull(const char * restrict nptr, char ** restrict endptr, int base) {
	(void)nptr;
	(void)endptr;
	(void)base;
	puts("strtoull: not implemented");
	return 0;
}

void free(void *ptr) {
	(void)ptr;
}

void * malloc(size_t size) {
	(void)size;
	return NULL;
}

_Noreturn void exit(int status) {
	_Exit(status);
}

_Noreturn void _Exit(int status) {
	_exit(status);
}

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
	for(bool done = true; !done; done = true) {
		for(size_t i = 0; i < nmemb - 1; ++i) {
			unsigned char *s1 = (unsigned char *)base + size * i;
			unsigned char *s2 = s1 + size;

			int result = compar(s1, s2);
			if(result > 0) {
				for(size_t n = 0; n < size; ++n) {
					unsigned char tmp = s1[n];
					s1[n] = s2[n];
					s2[n] = tmp;
				}
				done = false;
			}
		}
	}
}
