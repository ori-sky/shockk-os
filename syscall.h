#ifndef SYSCALL_H
#define SYSCALL_H

enum {
	SYSCALL_COMMAND_PUT
};

#define syscall_put(C) __asm__ __volatile__ ("int $0x80" : : "a" (SYSCALL_COMMAND_PUT), "b" (C))

#endif
