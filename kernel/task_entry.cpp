#include <kernel/elf.h>
#include <kernel/panic.h>
#include <kernel/screen.h>
#include <kernel/state.h>
#include <kernel/task.h>
#include <kernel/user.h>

void task_entry(Task *task) {
	__asm__ ("sti");

	screen_print("spawned new task ");
	screen_print(task->exe_name);
	screen_put('\n');

	_kernel_state.pager->Enable(task->context);
	user_enter(task->entry, &task->stack[PAGE_ALLOCATOR_PAGE_SIZE * Task::STACK_PAGES]);

	for(;;) { __asm__ ("hlt"); } // unreachable
}
