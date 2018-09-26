#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

enum {
	SYSCALL_COMMAND_OPEN,
	SYSCALL_COMMAND_GET,
	SYSCALL_COMMAND_PUT
};

// FD = open(PATH)
#define syscall_open(FD, PATH) __asm__ __volatile \
	("int $0x80" : "=a" (FD) : "a" (SYSCALL_COMMAND_OPEN), "b" (PATH))

// C = get(FD)
#define syscall_get(C, FD) __asm__ __volatile__ \
	("int $0x80" : "=a" (C) : "a" (SYSCALL_COMMAND_GET), "b" (FD))

// put(C, FD)
#define syscall_put(C, FD) __asm__ __volatile__ \
	("int $0x80" : : "a" (SYSCALL_COMMAND_PUT), "b" (C), "c" (FD))

#endif
