#include <kernel/syscall.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

register caddr_t stack_ptr asm ("sp");

void _exit();

int close(int file) {
	char sz[] = "NOT IMPLEMENTED: close\n";
	write(stdout, sz, sizeof(sz) - 1);

	return -1;
}

char *__env[1] = {0};
char **environ = __env;

int execve(char *name, char **argv, char **env) {
	char sz[] = "NOT IMPLEMENTED: execve\n";
	write(stdout, sz, sizeof(sz) - 1);

	errno = ENOMEM;
	return -1;
}

int fork() {
	char sz[] = "NOT IMPLEMENTED: fork\n";
	write(stdout, sz, sizeof(sz) - 1);

	errno = EAGAIN;
	return -1;
}

int fstat(int file, struct stat *st) {
	char sz[] = "NOT IMPLEMENTED: fstat\n";
	write(stdout, sz, sizeof(sz) - 1);

	st->st_mode = S_IFCHR;
	return 0;
}

int getpid() {
	char sz[] = "NOT IMPLEMENTED: getpid\n";
	write(stdout, sz, sizeof(sz) - 1);

	return 1;
}

int isatty(int file) {
	char sz[] = "NOT IMPLEMENTED: isatty\n";
	write(stdout, sz, sizeof(sz) - 1);

	return 1;
}

int kill(int pid, int sig) {
	char sz[] = "NOT IMPLEMENTED: kill\n";
	write(stdout, sz, sizeof(sz) - 1);

	errno = EINVAL;
	return -1;
}

int link(char *old, char *new) {
	char sz[] = "NOT IMPLEMENTED: link\n";
	write(stdout, sz, sizeof(sz) - 1);

	errno = EMLINK;
	return -1;
}

int lseek(int file, int ptr, int dir) {
	char sz[] = "NOT IMPLEMENTED: lseek\n";
	write(stdout, sz, sizeof(sz) - 1);

	return 0;
}

int open(const char *name, int flags, ...) {
	char sz[] = "NOT IMPLEMENTED: open\n";
	write(stdout, sz, sizeof(sz) - 1);

	return -1;
}

int read(int file, char *ptr, int len) {
	char sz[] = "NOT IMPLEMENTED: read\n";
	write(stdout, sz, sizeof(sz) - 1);

	return 0;
}

// reserve 32K for stack
#define STACK_BUFFER (4096 * 32)

caddr_t sbrk(int incr) {
	char sz[] = "NOT IMPLEMENTED: sbrk\n";
	write(stdout, sz, sizeof(sz) - 1);

	extern char _end;

	static caddr_t heap_ptr = (caddr_t)&_end;

	if((stack_ptr - (heap_ptr + incr)) > STACK_BUFFER) {
		caddr_t base = heap_ptr;
		heap_ptr += incr;
		return base;
	} else {
		errno = ENOMEM;
		return (caddr_t)-1;
	}
}

/*
caddr_t sbrk(int incr) {
	char sz[] = "NOT IMPLEMENTED: sbrk\n";
	write(stdout, sz, sizeof(sz) - 1);

	extern char _end;
	static char *heap_end;
	char *prev_heap_end;

	if(heap_end == 0) {
		heap_end = &_end;
	}
	prev_heap_end = heap_end;

	if(heap_end + incr > stack_ptr) {
		write(1, "Heap and stack collision\n", 25);
		errno = ENOMEM;
		return (caddr_t)-1;
	}

	heap_end += incr;
	return (caddr_t)prev_heap_end;
}
*/

int stat(const char *file, struct stat *st) {
	char sz[] = "NOT IMPLEMENTED: stat\n";
	write(stdout, sz, sizeof(sz) - 1);

	st->st_mode = S_IFCHR;
	return 0;
}

clock_t times(struct tms *buf) {
	char sz[] = "NOT IMPLEMENTED: times\n";
	write(stdout, sz, sizeof(sz) - 1);

	return -1;
}

int unlink(char *name) {
	char sz[] = "NOT IMPLEMENTED: unlink\n";
	write(stdout, sz, sizeof(sz) - 1);

	errno = ENOENT;
	return -1;
}

int wait(int *status) {
	char sz[] = "NOT IMPLEMENTED: wait\n";
	write(stdout, sz, sizeof(sz) - 1);

	errno = ECHILD;
	return -1;
}

int write(int file, char *ptr, int len) {
	for(int n = 0; n < len; ++n) {
		syscall_put(ptr[n]);
	}
	errno = EIO;
	return -1;
}

//int gettimeofday(struct timeval * __restrict__ p, void *z);
