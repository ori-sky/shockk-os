#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/task.h>

Task * Task::Create(const char *exe_name) {
	Task *task = (Task *)_kernel_state.pager->GetContext().Reserve();

	task->context = _kernel_state.pager->MakeContext();

	task->kernel_stack = (unsigned char *)task->context.Reserve() + PAGE_ALLOCATOR_PAGE_SIZE;
	task->kernel_esp = task->kernel_stack;

	// copy exe_name to task structure page
	size_t n;
	for(n = 0; n < sizeof(task->exe_name) - 1 && exe_name[n] != '\0'; ++n) {
		task->exe_name[n] = exe_name[n];
	}
	task->exe_name[n] = '\0';

	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = (uint32_t)task->exe_name; // argument
	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = 0;                        // ret addr for kernel_entry
	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = (uint32_t)&task_entry;    // ret addr for task_switch
	task->kernel_esp -= 4*4;                                  // regs pushed by task_switch

	task->stack = (unsigned char *)task->context.Alloc();
	for(size_t n = 1; n < STACK_PAGES; ++n) {
		task->context.AllocAt(&task->stack[PAGE_ALLOCATOR_PAGE_SIZE*n]);
	}

	return task;
}
