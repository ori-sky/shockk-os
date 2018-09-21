#include <dirent.h>
#include <stdio.h>

int closedir(DIR *dirp) {
	(void)dirp;
	puts("closedir: not implemented");
	return -1;
}

DIR * opendir(const char *dirname) {
	(void)dirname;
	puts("opendir: not implemented");
	return NULL;
}

struct dirent * readdir(DIR *dirp) {
	(void)dirp;
	puts("readdir: not implemented");
	return NULL;
}
