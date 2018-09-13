#include <errno.h>
#include <unistd.h>
#include <kernel/syscall.h>
/*
#include <sys/stat.h>
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

void _exit();

int close(int file) {
	char sz[] = "NOT IMPLEMENTED: close\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	return -1;
}

char *__env[1] = {0};
char **environ = __env;

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

int getpid() {
	char sz[] = "NOT IMPLEMENTED: getpid\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	return 1;
}

int isatty(int file) {
	char sz[] = "NOT IMPLEMENTED: isatty\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	return 1;
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

int open(const char *name, int flags, ...) {
	char sz[] = "NOT IMPLEMENTED: open\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	return -1;
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

int stat(const char *file, struct stat *st) {
	char sz[] = "NOT IMPLEMENTED: stat\n";
	write(STDERR_FILENO, sz, sizeof(sz) - 1);

	st->st_mode = S_IFCHR;
	return 0;
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

ssize_t read(int filedes, void *buf, size_t nbyte) {
	if(filedes == STDIN_FILENO) {
		char *sz = buf;
		for(size_t i = 0; i < nbyte; ++i) {
			syscall_get(sz[i]);
		}
		return nbyte;
	} else {
		errno = EAGAIN;
		return -1;
	}
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
