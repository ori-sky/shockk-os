void kernel_panic(const char *s);

void loader_entry(void) __attribute__((noreturn));
void loader_entry(void) {
	screen_init();
	screen_print("Hello, world!");
	for(;;) { __asm__ ("hlt"); }
}
