#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <kernel/pager.h>

class Task {
private:
public:
	static constexpr size_t STACK_PAGES = 64;

	Pager::Context context;
	unsigned char *kernel_stack;
	unsigned char *stack;

	Task(void);
};

#endif
