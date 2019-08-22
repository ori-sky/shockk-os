#include <kernel/elf.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/task.h>

Task * Task::Create(const char *path) {
	Task *task = (Task *)_kernel_state.pager->GetContext().Reserve();
	return task->Exec(path) ? task : nullptr;
}

Task * Task::Fork(void) {
	Task *task = (Task *)_kernel_state.pager->GetContext().Reserve();

	task->context = _kernel_state.pager->MakeContext();

	unsigned char *self_kernel_stack_page = kernel_stack - PAGE_ALLOCATOR_PAGE_SIZE;
	unsigned char *task_kernel_stack_page = (unsigned char *)task->context.Reserve();

	task->kernel_stack = task_kernel_stack_page + PAGE_ALLOCATOR_PAGE_SIZE;
	task->kernel_esp = task->kernel_stack;

	// copy exe_name to task structure page
	size_t n;
	for(n = 0; n < sizeof(task->exe_name) - 1 && exe_name[n] != '\0'; ++n) {
		task->exe_name[n] = exe_name[n];
	}
	task->exe_name[n] = '\0';

	auto diff = kernel_stack - kernel_esp;
	task->kernel_esp -= diff;

	for(size_t off = 0; off < PAGE_ALLOCATOR_PAGE_SIZE; ++off) {
		task_kernel_stack_page[off] = self_kernel_stack_page[off];
	}

	return task;
}

bool Task::Exec(const char *path) {
	auto new_context = _kernel_state.pager->MakeContext();

	// find inode

	char buffer[512];

	auto mData = _kernel_state.fs.GetInode(2);
	if(mData.IsNothing()) { return false; }
	auto data = mData.FromJust();

	size_t off = 1;
	for(size_t i = off; ; ++i) {
		buffer[i - off] = path[i];

		if(path[i] == '/') {
			buffer[i - off] = '\0';

			mData = _kernel_state.fs.GetInode(data, buffer);
			if(mData.IsNothing()) { return false; }
			data = mData.FromJust();

			off = i + 1;
		} else if(path[i] == '\0') {
			mData = _kernel_state.fs.GetInode(data, buffer);
			if(mData.IsNothing()) { return false; }
			data = mData.FromJust();

			break;
		}
	}

	// set new context

	context = new_context;

	// copy path to task structure page

	size_t n;
	for(n = 0; n < sizeof(exe_name) - 1 && path[n] != '\0'; ++n) {
		exe_name[n] = path[n];
	}
	exe_name[n] = '\0';

	// set up stack

	kernel_stack = (unsigned char *)context.Reserve() + PAGE_ALLOCATOR_PAGE_SIZE;
	kernel_esp = kernel_stack;

	kernel_esp -= 4;
	*(uint32_t *)kernel_esp = (uint32_t)this;           // argument
	kernel_esp -= 4;
	*(uint32_t *)kernel_esp = 0;                        // ret addr for kernel_entry
	kernel_esp -= 4;
	*(uint32_t *)kernel_esp = (uint32_t)&task_entry;    // ret addr for task_switch
	kernel_esp -= 4*4;                                  // regs pushed by task_switch

	stack = (unsigned char *)context.Alloc();
	for(size_t n = 1; n < STACK_PAGES; ++n) {
		context.AllocAt(&stack[PAGE_ALLOCATOR_PAGE_SIZE*n]);
	}

	// load elf

	ELF elf(context, data);
	entry = elf.entry();

	return true;
}

void task_switch(Task *curr, Task *next) {
	__asm__ ("cli");
	_kernel_state.task = next;
	_task_switch(_kernel_state.tss, curr, next);
	__asm__ ("sti");
}
