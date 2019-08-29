#ifndef _FNMATCH_H
#define _FNMATCH_H 1

enum {
	FNM_NOMATCH,
	FNM_ERROR
};

#define FNM_PATHNAME 0x1
#define FNM_PERIOD   0x2
#define FNM_NOESCAPE 0x4

int fnmatch(const char *pattern, const char *string, int flags);

#endif
