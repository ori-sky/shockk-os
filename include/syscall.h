#ifndef SYSCALL_H
#define SYSCALL_H

#include <cpu.h>

struct syscall
{
	int num;
	int params[6];
};

typedef int (*syscall_t)(struct syscall);

extern void syscall_handler(struct cpu_state *);

#endif
