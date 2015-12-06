#include <kernel/screen.h>
#include <kernel/panic.h>

void kernel_panic(const char *s) {
	screen_print("[PANIC] ");
	screen_print(s);
	screen_put('\n');
	__asm__ ("cli");
	for(;;) { __asm__ ("hlt"); }
}
