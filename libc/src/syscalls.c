#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <kernel/syscall.h>
/*
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <stdio.h>
#include <stdlib.h>
*/

/*
register void *stack_ptr asm ("sp");

int execve(const char *name, char * const argv[], char * const env[]) {
	char sz[] = "NOT IMPLEMENTED: execve\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	errno = ENOMEM;
	return -1;
}

int fork() {
	char sz[] = "NOT IMPLEMENTED: fork\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	errno = EAGAIN;
	return -1;
}

int fstat(int file, struct stat *st) {
	char sz[] = "NOT IMPLEMENTED: fstat\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	st->st_mode = S_IFCHR;
	return 0;
}

int kill(int pid, int sig) {
	char sz[] = "NOT IMPLEMENTED: kill\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	errno = EINVAL;
	return -1;
}

int link(const char *old, const char *new) {
	char sz[] = "NOT IMPLEMENTED: link\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	errno = EMLINK;
	return -1;
}

off_t lseek(int file, off_t ptr, int dir) {
	char sz[] = "NOT IMPLEMENTED: lseek\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	return 0;
}

// reserve 32K for stack
#define STACK_BUFFER (4096 * 32)

void * sbrk(ptrdiff_t incr) {
	char sz[] = "NOT IMPLEMENTED: sbrk\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	extern char _end;

	static void *heap_ptr = (void *)&_end;

	if((stack_ptr - (heap_ptr + incr)) > STACK_BUFFER) {
		void *base = heap_ptr;
		heap_ptr += incr;
		return base;
	} else {
		errno = ENOMEM;
		return (void *)-1;
	}
}

clock_t times(struct tms *buf) {
	char sz[] = "NOT IMPLEMENTED: times\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	return -1;
}

int unlink(const char *name) {
	char sz[] = "NOT IMPLEMENTED: unlink\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	errno = ENOENT;
	return -1;
}

int wait(int *status) {
	char sz[] = "NOT IMPLEMENTED: wait\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	errno = ECHILD;
	return -1;
}
*/

char *__env[1] = {0};
char **environ = __env;

int close(int filedes) {
	(void)filedes;
	puts("close: not implemented");
	return -1;
}

int dup2(int filedes, int filedes2) {
	(void)filedes;
	(void)filedes2;
	puts("dup2: not implemented");
	return -1;
}

_Noreturn void _exit(int status) {
	fprintf(stderr, "return code = %d\n", status);
	fflush(stdout);
	fflush(stderr);
	for(;;);
}

int isatty(int filedes) {
	(void)filedes;
	puts("isatty: not implemented");
	return 1;
}

gid_t getegid(void) {
	puts("getegid: not implemented");
	return 1;
}

uid_t geteuid(void) {
	puts("geteuid: not implemented");
	return 1;
}

gid_t getgid(void) {
	puts("getgid: not implemented");
	return 1;
}

int getgroups(int gidgetsize, gid_t grouplist[]) {
	(void)gidgetsize;
	(void)grouplist;
	puts("getgroups: not implemented");
	return -1;
}

pid_t getpid(void) {
	puts("getpid: not implemented");
	return 1;
}

pid_t getppid(void) {
	puts("getppid: not implemented");
	return 1;
}

uid_t getuid(void) {
	puts("getuid: not implemented");
	return 1;
}

int open(const char *path, int oflag, ...) {
	(void)oflag;
	int fd;
	syscall_open(fd, path);
	return fd;
}

int pipe(int filedes[2]) {
	(void)filedes;
	puts("pipe: not implemented");
	return -1;
}

ssize_t read(int filedes, void *buf, size_t nbyte) {
	char *sz = buf;
	for(size_t i = 0; i < nbyte; ++i) {
		syscall_get(sz[i], filedes);
	}
	return nbyte;
}

int lstat(const char * restrict file, struct stat * restrict st) {
	(void)file;
	(void)st;
	puts("lstat: not implemented");
	return -1;
}

int stat(const char * restrict file, struct stat * restrict st) {
	(void)file;
	(void)st;
	puts("stat: not implemented");
	return -1;
}

pid_t tcgetpgrp(int filedes) {
	(void)filedes;
	puts("tcgetpgrp: not implemented");
	return -1;
}

int tcsetpgrp(int filedes, pid_t pgid_id) {
	(void)filedes;
	(void)pgid_id;
	puts("tcsetpgrp: not implemented");
	return -1;
}

mode_t umask(mode_t cmask) {
	(void)cmask;
	puts("umask: not implemented");
	return 0;
}

pid_t waitpid(pid_t pid, int *stat_loc, int options) {
	(void)pid;
	(void)stat_loc;
	(void)options;
	puts("waitpid: not implemented");
	return -1;
}

ssize_t write(int filedes, const void *buf, size_t nbyte) {
	if(filedes == STDOUT_FILENO || filedes == STDERR_FILENO) {
		const char *sz = buf;
		for(size_t i = 0; i < nbyte; ++i) {
			syscall_put(sz[i]);
		}
		return nbyte;
	} else {
		errno = EAGAIN;
		return -1;
	}
}

//int gettimeofday(struct timeval * __restrict__ p, void *z);
