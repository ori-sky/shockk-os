#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
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

int chdir(const char *path) {
	(void)path;
	puts("chdir: not implemented");
	return -1;
}

int close(int filedes) {
	(void)filedes;
	puts("close: not implemented");
	errno = EBADF;
	return -1;
}

int dup2(int filedes, int filedes2) {
	(void)filedes;
	(void)filedes2;
	puts("dup2: not implemented");
	return -1;
}

int execve(const char *path, char * const argv[], char * const envp[]) {
	(void)argv;
	(void)envp;
	syscall_exec(path); // noreturn if successful
	errno = EACCES;
	return -1;
}

_Noreturn void _exit(int status) {
	fprintf(stderr, "return code = %d\n", status);
	fflush(stdout);
	fflush(stderr);
	for(;;) {
		syscall_exit();
	}
}

int fcntl(int filedes, int cmd, ...) {
	(void)filedes;
	(void)cmd;
	puts("fcntl: not implemented");
	return -1;
}

pid_t fork(void) {
	pid_t pid = 0;
	syscall_fork(pid);
	return pid;
}

int fstat(int filedes, struct stat *buf) {
	(void)filedes;
	(void)buf;
	puts("fstat: not implemented");
	return 0;
}

gid_t getegid(void) {
	gid_t egid;
	syscall_getegid(egid);
	return egid;
}

uid_t geteuid(void) {
	uid_t euid;
	syscall_geteuid(euid);
	return euid;
}

gid_t getgid(void) {
	gid_t gid;
	syscall_getgid(gid);
	return gid;
}

int getgroups(int gidgetsize, gid_t grouplist[]) {
	(void)gidgetsize;
	(void)grouplist;
	puts("getgroups: not implemented");
	return -1;
}

pid_t getpgrp(void) {
	puts("getpgrp: not implemented");
	return 1;
}

pid_t getpid(void) {
	pid_t pid;
	syscall_getpid(pid);
	return pid;
}

pid_t getppid(void) {
	pid_t ppid;
	syscall_getppid(ppid);
	return ppid;
}

uid_t getuid(void) {
	uid_t uid;
	syscall_getuid(uid);
	return uid;
}

int isatty(int filedes) {
	if(filedes == STDIN_FILENO || filedes == STDOUT_FILENO || filedes == STDERR_FILENO) {
		return 1;
	} else {
		printf("isatty: not implemented for %d\n", filedes);
		return 0;
	}
}

int lstat(const char * restrict file, struct stat * restrict st) {
	(void)file;
	(void)st;
	puts("lstat: not implemented");
	return -1;
}

void * mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off) {
	if(addr != NULL) {
		puts("mmap: non-null addr not supported");
		return MAP_FAILED;
	}
	if(fildes != 0) {
		puts("mmap: fildes not supported");
		return MAP_FAILED;
	}

	(void)prot;
	(void)flags;
	(void)off;

	static void *next_addr = (void *)0x1000000;

	if(next_addr > (void *)0x2000000) {
		puts("mmap: ran out of heap space");
		return MAP_FAILED;
	}

	//printf("mmap: allocating %d bytes at %d\n", (int)len, (int)next_addr);

	addr = next_addr;
	next_addr += len;
	return addr;
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
	size_t n = 0;
	for(n = 0; n < nbyte; ++n) {
		do {
			syscall_get(sz[n], filedes);
		} while(sz[n] == '\0');

		// POSIX -> 11. General Terminal Interface -> Canonical Mode Input Processing
		if(sz[n] == '\n') {
			++n;
			break;
		}
	}
	return n;
}

int setpgid(pid_t pid, pid_t pgid) {
	(void)pid;
	(void)pgid;
	puts("setpgid: not implemented");
	return -1;
}

int stat(const char * restrict file, struct stat * restrict st) {
	//printf("stat: not implemented: %s\n", file);
	(void)file;

	st->st_dev          = 0;
	st->st_ino          = 50;
	st->st_mode         = S_IRUSR | S_IWUSR;
	st->st_nlink        = 1;
	st->st_uid          = 1;
	st->st_gid          = 1;
	st->st_atim.tv_sec  = 0;
	st->st_atim.tv_nsec = 0;
	st->st_mtim.tv_sec  = 0;
	st->st_mtim.tv_nsec = 0;
	st->st_ctim.tv_sec  = 0;
	st->st_ctim.tv_nsec = 0;

	return 0;

	//errno = ENOENT;
	//return -1;
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
	if(options > (WCONTINUED | WNOHANG | WUNTRACED)) {
		errno = EINVAL;
		return -1;
	}

	if(options > 0) {
		puts("waitpid: options not implemented");
		errno = EINVAL;
		return -1;
	}

	if(pid > 0) {
		puts("waitpid: pid > 0 not implemented");
		errno = ECHILD;
		return -1;
	} else if(pid == 0) {
		puts("waitpid: pid == 0 not implemented");
		errno = ECHILD;
		return -1;
	} else if(pid == (pid_t)-1) {
		pid_t ret;
		int stat;
		syscall_waitpid(ret, stat, pid);

		if(ret == -1) {
			errno = ECHILD;
		}

		if(stat_loc != NULL) {
			*stat_loc = stat;
		}
		return ret;
	} else /*if(pid < (pid_t)-1)*/ {
		puts("waitpid: pid < -1 not implemented");
		errno = ECHILD;
		return -1;
	}
}

ssize_t write(int filedes, const void *buf, size_t nbyte) {
	if(filedes == STDOUT_FILENO || filedes == STDERR_FILENO) {
		const char *sz = buf;
		for(size_t i = 0; i < nbyte; ++i) {
			syscall_put(sz[i], filedes);
		}
		return nbyte;
	} else {
		errno = EAGAIN;
		return -1;
	}
}

//int gettimeofday(struct timeval * __restrict__ p, void *z);
