#ifndef _SYS_UTSNAME_H
#define _SYS_UTSNAME_H 1

#define MAX_UTSNAME 256

struct utsname {
	char sysname[MAX_UTSNAME];
	char nodename[MAX_UTSNAME];
	char release[MAX_UTSNAME];
	char version[MAX_UTSNAME];
	char machine[MAX_UTSNAME];
};

#endif
