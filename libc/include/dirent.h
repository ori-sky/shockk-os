#ifndef _DIRENT_H
#define _DIRENT_H 1

#include <limits.h>
#include <sys/types.h>

typedef struct {} DIR;

struct dirent {
	ino_t d_ino;
	char d_name[NAME_MAX + 1];
};

int closedir(DIR *);
DIR * opendir(const char *);
struct dirent * readdir(DIR *);

#endif
