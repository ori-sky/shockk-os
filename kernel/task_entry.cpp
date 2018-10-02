#include <kernel/elf.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/task.h>
#include <kernel/user.h>

void task_entry(const char *) {
	screen_print("spawned new task\n");

	const char *paths[] = {"bin", "two.elf"};
	auto mData = _kernel_state.fs.GetInode(2, paths);
	if(mData.IsNothing()) { kernel_panic("failed to get /bin/two.elf inode"); }
	auto data = mData.FromJust();

	_kernel_state.pager->Enable(_kernel_state.task->context);
	ELF elf(data);

	user_enter(elf.entry(), &_kernel_state.task->stack[PAGE_ALLOCATOR_PAGE_SIZE * Task::STACK_PAGES]);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
