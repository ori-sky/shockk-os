#include <ctype.h>
#include <stddef.h>
#include <strings.h>

int strcasecmp(const char *s1, const char *s2) {
	for(size_t i = 0; s1[i] != '\0'; ++i) {
		if(toupper(s1[i]) < toupper(s2[i])) { return -1; }
		if(toupper(s1[i]) > toupper(s2[i])) { return  1; }
	}
	return 0;
}

int strncasecmp(const char *s1, const char *s2, size_t n) {
	for(size_t i = 0; i < n && s1[i] != '\0'; ++i) {
		if(toupper(s1[i]) < toupper(s2[i])) { return -1; }
		if(toupper(s1[i]) > toupper(s2[i])) { return  1; }
	}
	return 0;
}
