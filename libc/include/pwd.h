#ifndef _PWD_H
#define _PWD_H 1

#include <sys/types.h>

struct passwd {
	char *pw_name;
	uid_t pw_uid;
	gid_t pw_gid;
	char *pw_dir;
	char *pw_shell;
};

#endif
