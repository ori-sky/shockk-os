#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <syscall.h>

extern int sc_exit(struct syscall);
extern int sc_close(struct syscall);
extern int sc_execve(struct syscall);
extern int sc_fork(struct syscall);
extern int sc_fstat(struct syscall);
extern int sc_getpid(struct syscall);
extern int sc_isatty(struct syscall);
extern int sc_kill(struct syscall);
extern int sc_link(struct syscall);
extern int sc_lseek(struct syscall);
extern int sc_open(struct syscall);
extern int sc_read(struct syscall);
extern int sc_sbrk(struct syscall);
extern int sc_stat(struct syscall);
extern int sc_times(struct syscall);
extern int sc_unlink(struct syscall);
extern int sc_wait(struct syscall);
extern int sc_write(struct syscall);

extern const syscall_t syscalls[20];

#endif
