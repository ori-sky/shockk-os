#include <kernel/elf.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/task.h>
#include <kernel/user.h>

void task_entry(const char *exe_name) {
	__asm__ ("sti");

	screen_print("spawned new task\n");
	screen_print("loading ");
	screen_print(exe_name);
	screen_put('\n');

	const char *paths[] = {"bin", exe_name};
	auto mData = _kernel_state.fs.GetInode(2, paths);
	if(mData.IsNothing()) { kernel_panic("failed to get inode"); }
	auto data = mData.FromJust();

	_kernel_state.pager->Enable(_kernel_state.task->context);
	ELF elf(data);

	user_enter(elf.entry(), &_kernel_state.task->stack[PAGE_ALLOCATOR_PAGE_SIZE * Task::STACK_PAGES]);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
