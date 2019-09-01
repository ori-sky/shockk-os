//#include_next <limits.h>

#ifndef _SHK_LIMITS_H
#define _SHK_LIMITS_H 1

#ifndef _LIMITS_H
#define _LIMITS_H 1
#endif

#define _POSIX_ARG_MAX  4096
#define _POSIX_NAME_MAX 14
#define _POSIX_PATH_MAX 256

#define ARG_MAX  _POSIX_ARG_MAX
#define NAME_MAX _POSIX_NAME_MAX
#define PATH_MAX _POSIX_PATH_MAX

#endif
