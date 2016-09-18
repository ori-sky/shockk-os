#include <kernel/syscall.h>

extern "C" void user_entry(void) __attribute__((noreturn));
void user_entry(void) {
	for(;;);
	//syscall_put(':');
	//syscall_put('D');
	//syscall_put('\n');
	//syscall_put('\n');
	//for(;;);
}
