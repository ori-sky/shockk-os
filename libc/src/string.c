#include <string.h>

char * strcpy(char * restrict s1, const char * restrict s2) {
	for(size_t i = 0; s2[i] != '\0'; ++i) {
		s1[i] = s2[i];
	}
	return s1;
}

size_t strlen(const char *s) {
	size_t n;
	for(n = 0; s[n] != '\0'; ++n) {}
	return n;
}
