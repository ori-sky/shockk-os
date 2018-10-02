#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <kernel/pager.h>
#include <kernel/tss.h>

class Task {
private:
	Task(void) = default;
public:
	unsigned char *kernel_esp;
	unsigned char *kernel_stack;
	unsigned char *stack;
	Task *next = nullptr;
	Pager::Context context;
	char exe_name[512];

	static constexpr size_t STACK_PAGES = 64;

	static Task * Create(const char *);
};

extern "C" void task_switch(TSS *, Task *, Task *);
extern "C" void task_entry(const char *) __attribute__((noreturn));

#endif
