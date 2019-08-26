#include <kernel/elf.h>
#include <kernel/new.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/task.h>

Task * Task::Create(const char *path) {
	auto ptr = _kernel_state.pager->GetContext().Reserve();
	auto task = new (ptr) Task(_kernel_state.next_pid);
	if(task->Exec(path)) {
		++_kernel_state.next_pid;
		_kernel_state.pids[task->pid] = task;
		return task;
	} else {
		return nullptr;
	}
}

Task * Task::Fork(uint32_t ebp, IRETState iret) {
	auto ptr = _kernel_state.pager->GetContext().Reserve();
	auto task = new (ptr) Task(_kernel_state.next_pid++);
	_kernel_state.pids[task->pid] = task;

	ptr = _kernel_state.pager->GetContext().Reserve();
	child = new (ptr) Child;
	child->task = task;

	task->context = _kernel_state.pager->ForkContext(context);

	// copy exe_name to task structure page
	size_t n;
	for(n = 0; n < sizeof(task->exe_name) - 1 && exe_name[n] != '\0'; ++n) {
		task->exe_name[n] = exe_name[n];
	}
	task->exe_name[n] = '\0';

	// set up stack

	task->kernel_stack = (unsigned char *)task->context.Reserve() + PAGE_ALLOCATOR_PAGE_SIZE;
	task->kernel_esp = task->kernel_stack;

	// IRETState
	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = iret.ss;
	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = iret.esp;
	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = iret.eflags;
	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = iret.cs;
	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = iret.eip;

	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = ebp;                      // ebp
	task->kernel_esp -= 4;
	*(uint32_t *)task->kernel_esp = (uint32_t)&_task_fork;    // ret addr for task_fork

	task->kernel_esp -= 4*4;                                  // regs pushed by task_switch

	task->stack = stack;

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
	// if next task is dead, skip it
	if(!next->running && next->next != nullptr) {
		next = next->next;
	}

	__asm__ ("cli");
	_kernel_state.task = next;
	_task_switch(_kernel_state.tss, curr, next);
	__asm__ ("sti");
}
