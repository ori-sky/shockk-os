#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <kernel/pager.h>
#include <kernel/cpu.h>
#include <kernel/tss.h>

class Task {
private:
	Task(void) = default;
public:
	static constexpr size_t STACK_PAGES = 64;

	unsigned char *kernel_esp;
	unsigned char *kernel_stack;
	unsigned char *stack;
	Task *next = nullptr;
	Pager::Context context;

	void (*entry)();
	uint32_t pid;
	char exe_name[512];
	bool running;

	static Task * Create(const char *);

	Task * Fork(uint32_t, IRETState) const;
	bool Exec(const char *);
};

extern "C" void _task_switch(TSS *, Task *, Task *);
extern "C" void task_switch(Task *, Task *);

extern "C" void _task_fork(TSS *, Task *, Task *);

extern "C" void task_entry(Task *) __attribute__((noreturn));

#endif
