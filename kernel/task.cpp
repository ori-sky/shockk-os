#include <kernel/state.h>
#include <kernel/task.h>

Task * Task::Create(void) {
	Task *task = (Task *)_kernel_state.pager->GetContext().Reserve();

	task->context = _kernel_state.pager->MakeContext();

	task->kernel_stack = (unsigned char *)task->context.Reserve() + PAGE_ALLOCATOR_PAGE_SIZE;
	task->kernel_esp = task->kernel_stack;

	task->kernel_esp -= 4;
	*(int *)task->kernel_esp = (int)&task_entry;
	task->kernel_esp -= 4*4;

	task->stack = (unsigned char *)task->context.Alloc();
	for(size_t n = 1; n < STACK_PAGES; ++n) {
		task->context.AllocAt(&task->stack[PAGE_ALLOCATOR_PAGE_SIZE*n]);
	}

	return task;
}
