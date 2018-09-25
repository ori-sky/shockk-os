#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int atoi(const char *nptr) {
	return (int)strtol(nptr, (char **)NULL, 10);
}

long int strtol(const char * restrict nptr, char ** restrict endptr, int base) {
	(void)nptr;
	(void)endptr;
	(void)base;
	puts("strtol: not implemented");
	return 0;
}

long long int strtoll(const char * restrict nptr, char ** restrict endptr, int base) {
	if(base != 10) {
		puts("strtoll: base != 10");
		return 0;
	}

	(void)nptr;
	(void)endptr;
	(void)base;

	printf("strtoll: not implemented (%s)\n", nptr);

	if(endptr != NULL) {
		*endptr = (char *)&nptr[1];
	}
	return 1;
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
	puts("free: not implemented");
}

void * malloc(size_t size) {
	return mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
}

void * realloc(void *ptr, size_t size) {
	(void)ptr;
	(void)size;
	puts("realloc: not implemented");
	return NULL;
}

_Noreturn void abort(void) {
	puts("abort: not implemented");
	exit(1);
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
