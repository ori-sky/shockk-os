#include <kernel/state.h>
#include <kernel/task.h>

Task::Task(void) {
	context = _kernel_state.pager->MakeContext();

	kernel_stack = (unsigned char *)context.Reserve();

	stack = (unsigned char *)context.Alloc();
	for(size_t n = 1; n < STACK_PAGES; ++n) {
		context.AllocAt(&stack[PAGE_ALLOCATOR_PAGE_SIZE*n]);
	}
}
