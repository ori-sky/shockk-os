void loader_entry(void) __attribute__((noreturn));
void loader_entry(void) {
	for(;;) { __asm__ ("hlt"); }
}
