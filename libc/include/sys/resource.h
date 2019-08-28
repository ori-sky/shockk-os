#ifndef _SYS_RESOURCE_H
#define _SYS_RESOURCE_H 1

typedef unsigned int rlim_t;

#define RLIM_INFINITY  1
#define RLIM_SAVED_MAX 2
#define RLIM_SAVED_CUR 3

struct rlimit {
	rlim_t rlim_cur;
	rlim_t rlim_max;
};

struct rusage {
	rlim_t ru_utime;
	rlim_t ru_stime;
};

#endif
