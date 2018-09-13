#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

enum {
	SYSCALL_COMMAND_GET,
	SYSCALL_COMMAND_PUT
};

#define syscall_get(C) __asm__ __volatile__ ("int $0x80" : "=a" (C) : "a" (SYSCALL_COMMAND_GET))

#define syscall_put(C) __asm__ __volatile__ ("int $0x80" : : "a" (SYSCALL_COMMAND_PUT), "b" (C))

#endif
