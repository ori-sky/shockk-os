#include <kernel/syscall.h>

void user_entry(void) __attribute__((noreturn));
void user_entry(void) {
	syscall_put(':');
	syscall_put('D');
	syscall_put('\n');
	syscall_put('\n');
	for(;;);
}
