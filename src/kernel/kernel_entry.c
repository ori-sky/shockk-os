void kernel_entry(void) __attribute__((noreturn));
void kernel_entry(void) {
	for(;;) { __asm__ ("hlt"); }
}
