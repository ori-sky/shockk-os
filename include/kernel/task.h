#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <kernel/pager.h>
#include <kernel/cpu.h>
#include <kernel/tss.h>

#define ARG_MAX 4096

class Task {
private:
	Task(void) = default;
	Task(uint32_t pid) : pid(pid) {}
public:
	struct Child {
		Child *sibling;
		Task *task;
	};

	static constexpr size_t STACK_PAGES = 64;

	// XXX: DO NOT REORDER
	// the order of these members is important for _task_switch
	unsigned char *kernel_esp;
	unsigned char *kernel_stack;
	unsigned char *stack;
	Task *next = nullptr;
	Pager::Context context;

	// XXX: these members can be reordered as required
	Child *child = nullptr;
	void (*entry)() = nullptr;
	uint32_t pid;
	char exe_name[512];
	bool running = true;

	static Task * Create(const char *, char *[]);

	Task * Fork(uint32_t, IRETState);
	bool Exec(const char *, char *[]);
};

extern "C" void _task_switch(TSS *, Task *, Task *);
extern "C" void task_switch(Task *, Task *);

extern "C" void _task_fork(TSS *, Task *, Task *);

extern "C" void task_entry(Task *) __attribute__((noreturn));

#endif
