#ifndef _UNISTD_H
#define _UNISTD_H 1

#include <stddef.h>
#include <sys/types.h>

enum {
	F_OK,
	R_OK,
	W_OK,
	X_OK
};

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

char **environ;

int close(int);
int isatty(int);
gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);
int getgroups(int, gid_t[]);
pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);

#endif
