#include <string.h>

char * strcpy(char * restrict s1, const char * restrict s2) {
	for(size_t i = 0; s2[i] != '\0'; ++i) {
		s1[i] = s2[i];
	}
	return s1;
}

int strcmp(const char *s1, const char *s2) {
	for(size_t i = 0; s1[i] != '\0'; ++i) {
		if(s1[i] < s2[i]) { return -1; }
		if(s1[i] > s2[i]) { return  1; }
	}
	return 0;
}

char * strchr(const char *s, int c) {
	for(size_t i = 0; s[i] != '\0'; ++i) {
		if(s[i] == c) { return (char *)&s[i]; }
	}
	return NULL;
}

size_t strspn(const char *s1, const char *s2) {
	size_t n;
	for(n = 0; s1[n] != '\0' && s2[n] != '\0'; ++n) {
		if(s1[n] != s2[n]) { break; }
	}
	return n;
}

char * strerror(int errnum) {
	switch(errnum) {
	default:
		return "unknown errnum";
	}
}

size_t strlen(const char *s) {
	size_t n;
	for(n = 0; s[n] != '\0'; ++n) {}
	return n;
}

char * strsignal(int signum) {
	switch(signum) {
	default:
		return "unknown signum";
	}
}
