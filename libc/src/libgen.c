#include <libgen.h>
#include <string.h>

char dot_str[] = ".";

char * dirname(char *path) {
	if(path == NULL) {
		return dot_str;
	}

	char *r = strrchr(path, '/');

	if(r == NULL) {
		return dot_str;
	}

	while(r[1] == '\0') {
		r[0] = '\0';
		r = strrchr(path, '/');

		if(r == NULL) {
			return dot_str;
		}
	}

	r[0] = '\0';
	return path;
}
