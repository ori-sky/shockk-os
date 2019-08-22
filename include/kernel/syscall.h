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
	SYSCALL_COMMAND_PUT
};

// exec(PATH)
#define syscall_exec(PATH) __asm__ __volatile__ \
	("int $0x80" : : "a" (SYSCALL_COMMAND_EXEC), "b" (PATH))

// noreturn exit()
#define syscall_exit() __asm__ __volatile__ \
	("int $0x80" : : "a" (SYSCALL_COMMAND_EXIT))

// PID = fork()
#define syscall_fork(PID) __asm__ __volatile__ \
	("int $0x80" : "=a" (PID) : "a" (SYSCALL_COMMAND_FORK))

// C = get(FD)
#define syscall_get(C, FD) __asm__ __volatile__ \
	("int $0x80" : "=a" (C) : "a" (SYSCALL_COMMAND_GET), "b" (FD))

// EGID = getegid()
#define syscall_getegid(EGID) __asm__ __volatile__ \
	("int $0x80" : "=a" (EGID) : "a" (SYSCALL_COMMAND_GETEGID))

// EUID = geteuid()
#define syscall_geteuid(EUID) __asm__ __volatile__ \
	("int $0x80" : "=a" (EUID) : "a" (SYSCALL_COMMAND_GETEUID))

// GID = getgid()
#define syscall_getgid(GID) __asm__ __volatile__ \
	("int $0x80" : "=a" (GID) : "a" (SYSCALL_COMMAND_GETGID))

// PID = getpid()
#define syscall_getpid(PID) __asm__ __volatile__ \
	("int $0x80" : "=a" (PID) : "a" (SYSCALL_COMMAND_GETPID))

// PPID = getppid()
#define syscall_getppid(PPID) __asm__ __volatile__ \
	("int $0x80" : "=a" (PPID) : "a" (SYSCALL_COMMAND_GETPPID))

// UID = getuid()
#define syscall_getuid(UID) __asm__ __volatile__ \
	("int $0x80" : "=a" (UID) : "a" (SYSCALL_COMMAND_GETUID))

// FD = open(PATH)
#define syscall_open(FD, PATH) __asm__ __volatile \
	("int $0x80" : "=a" (FD) : "a" (SYSCALL_COMMAND_OPEN), "b" (PATH))

// put(C, FD)
#define syscall_put(C, FD) __asm__ __volatile__ \
	("int $0x80" : : "a" (SYSCALL_COMMAND_PUT), "b" (C), "c" (FD))

#endif
