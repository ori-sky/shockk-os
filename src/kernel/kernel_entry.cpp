#include <kernel/panic.h>

void kernel_entry(void) __attribute__((noreturn));
void kernel_entry(void) {
	kernel_panic("call to kernel_entry succeeded!");
	for(;;) { __asm__ ("hlt"); }
}
