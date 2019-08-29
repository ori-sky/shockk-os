#include <fnmatch.h>
#include <stdio.h>

int fnmatch(const char *pattern, const char *string, int flags) {
	(void)pattern;
	(void)string;
	(void)flags;
	puts("fnmatch: not implemented");
	return FNM_ERROR;
}
