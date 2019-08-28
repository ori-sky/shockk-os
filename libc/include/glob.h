#ifndef _GLOB_H
#define _GLOB_H 1

#include <sys/types.h>

struct glob {
	size_t gl_pathc;
	char **gl_pathv;
	size_t gl_offs;
};

typedef struct glob glob_t;

enum {
	GLOB_ABORTED,
	GLOB_NOMATCH,
	GLOB_NOSPACE
};

#endif
