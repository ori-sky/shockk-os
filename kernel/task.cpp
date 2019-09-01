#include <kernel/elf.h>
#include <kernel/new.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/task.h>

Task * Task::Create(const char *path, char *argv[]) {
	auto ptr = _kernel_state.pager->GetContext().Reserve();
	auto task = new (ptr) Task(_kernel_state.next_pid);
	if(task->Exec(path, argv)) {
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

bool Task::Exec(const char *path, char *argv[]) {
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

	auto old_context = _kernel_state.pager->GetContext();
	context = _kernel_state.pager->MakeContext();

	// copy path to task structure page

	size_t n;
	for(n = 0; n < sizeof(exe_name) - 1 && path[n] != '\0'; ++n) {
		exe_name[n] = path[n];
	}
	exe_name[n] = '\0';

	// set up kernel stack

	kernel_stack = (unsigned char *)context.Reserve() + PAGE_ALLOCATOR_PAGE_SIZE;
	kernel_esp = kernel_stack;

	kernel_esp -= 4;
	*(uint32_t *)kernel_esp = (uint32_t)this;           // argument
	kernel_esp -= 4;
	*(uint32_t *)kernel_esp = 0;                        // ret addr for kernel_entry
	kernel_esp -= 4;
	*(uint32_t *)kernel_esp = (uint32_t)&task_entry;    // ret addr for task_switch
	kernel_esp -= 4*4;                                  // regs pushed by task_switch

	// copy argv into kernel memory

	char buf[ARG_MAX];
	uint32_t *argvbuf = (uint32_t *)buf;

	off = 0;

	// increment offset by 4 for each pointer in argv
	size_t argc;
	for(argc = 0; argv[argc] != nullptr; ++argc) {
		off += 4;
	}

	// increment offset by 4 for terminating null pointer
	off += 4;

	for(size_t i = 0; argv[i] != nullptr; ++i) {
		// set arg offset
		argvbuf[i] = off;

		// copy arg
		size_t c = 0;
		do {
			if(off >= ARG_MAX) {
				kernel_panic("ARG_MAX exceeded");
			}

			buf[off++] = argv[i][c];
		} while(argv[i][c++] != '\0');
	}

	// set terminating null pointer
	argvbuf[argc] = 0;

	// set up user stack

	auto stack_base = (unsigned char *)context.Alloc();
	for(size_t n = 1; n < STACK_PAGES; ++n) {
		context.AllocAt(&stack_base[PAGE_ALLOCATOR_PAGE_SIZE*n]);
	}
	stack = &stack_base[PAGE_ALLOCATOR_PAGE_SIZE * STACK_PAGES];

	// copy argv to user stack

	stack -= ARG_MAX;

	_kernel_state.pager->Enable(context);

	for(size_t i = 0; i < ARG_MAX; ++i) {
		stack[i] = ((unsigned char *)buf)[i];
	}

	argvbuf = (uint32_t *)stack;

	for(size_t i = 0; i < argc; ++i) {
		argvbuf[i] += (uint32_t)argvbuf;
	}

	stack -= 4;
	*(uint32_t *)stack = (uint32_t)argvbuf; // argv argument
	stack -= 4;
	*(uint32_t *)stack = (uint32_t)nullptr; // _start return address

	_kernel_state.pager->Enable(old_context);

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
