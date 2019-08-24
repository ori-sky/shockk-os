#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

enum {
	SYSCALL_COMMAND_EXEC,
	SYSCALL_COMMAND_EXIT,
	SYSCALL_COMMAND_FORK,
	SYSCALL_COMMAND_GET,
	SYSCALL_COMMAND_GETEGID,
	SYSCALL_COMMAND_GETEUID,
	SYSCALL_COMMAND_GETGID,
	SYSCALL_COMMAND_GETPID,
	SYSCALL_COMMAND_GETPPID,
	SYSCALL_COMMAND_GETUID,
	SYSCALL_COMMAND_OPEN,
	SYSCALL_COMMAND_PUT,
	SYSCALL_COMMAND_WAITPID
};

#define syscall0(CMD, ARG1, ARG2, ARG3) \
	__asm__ __volatile__ \
	("int $0x80" : : "a" (CMD), "b" (ARG1), "c" (ARG2), "d" (ARG3))

#define syscall1(CMD, RET1, ARG1, ARG2, ARG3) \
	__asm__ __volatile__ \
	("int $0x80" : "=a" (RET1) : "a" (CMD), "b" (ARG1), "c" (ARG2), "d" (ARG3))

#define syscall2(CMD, RET1, RET2, ARG1, ARG2, ARG3) \
	__asm__ __volatile__ \
	("int $0x80" : "=a" (RET1), "=d" (RET2) : "a" (CMD), "b" (ARG1), "c" (ARG2), "d" (ARG3))

#define syscall_exec(PATH)              syscall0(SYSCALL_COMMAND_EXEC,               PATH, 0,  0)
#define syscall_exit()                  syscall0(SYSCALL_COMMAND_EXIT,               0,    0,  0)
#define syscall_fork(PID)               syscall1(SYSCALL_COMMAND_FORK,    PID,       0,    0,  0)
#define syscall_get(C, FD)              syscall1(SYSCALL_COMMAND_GET,     C,         FD,   0,  0)
#define syscall_getegid(EGID)           syscall1(SYSCALL_COMMAND_GETEGID, EGID,      0,    0,  0)
#define syscall_geteuid(EUID)           syscall1(SYSCALL_COMMAND_GETEUID, EUID,      0,    0,  0)
#define syscall_getgid(GID)             syscall1(SYSCALL_COMMAND_GETGID,  GID,       0,    0,  0)
#define syscall_getpid(PID)             syscall1(SYSCALL_COMMAND_GETPID,  PID,       0,    0,  0)
#define syscall_getppid(PPID)           syscall1(SYSCALL_COMMAND_GETPPID, PPID,      0,    0,  0)
#define syscall_getuid(UID)             syscall1(SYSCALL_COMMAND_GETUID,  UID,       0,    0,  0)
#define syscall_open(FD, PATH)          syscall1(SYSCALL_COMMAND_OPEN,    FD,        PATH, 0,  0)
#define syscall_put(C, FD)              syscall0(SYSCALL_COMMAND_PUT,                C,    FD, 0)
#define syscall_waitpid(RET, STAT, PID) syscall2(SYSCALL_COMMAND_WAITPID, RET, STAT, PID,  0,  0)

#endif
