#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

class Task {
private:
	Pager::Context context;
	unsigned char *stack;
public:
	Task() = default;
	Task(Pager::Context context, unsigned char *stack)
	   : context(context), stack(stack) {}
};

#endif
