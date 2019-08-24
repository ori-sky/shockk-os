#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H 1

#define WCONTINUED 1
#define WNOHANG    2
#define WUNTRACED  4

#define WEXITSTATUS(STATUS)   0
#define WIFCONTINUED(STATUS)  0
#define WIFEXITED(STATUS)     0
#define WIFSIGNALED(STATUS)   0
#define WIFSTOPPED(STATUS)    0
#define WSTOPSIG(STATUS)      0
#define WTERMSIG(STATUS)      0

enum {
	WEXITED,
	WNOWAIT,
	WSTOPPED
};

pid_t waitpid(pid_t, int *, int);

#endif
