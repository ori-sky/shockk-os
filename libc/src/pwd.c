#include <pwd.h>
#include <stdio.h>

struct passwd * getpwnam(const char *name) {
	(void)name;
	puts("getpwnam: not implemented");
	return NULL;
}
