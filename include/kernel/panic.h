#ifndef PANIC_H
#define PANIC_H

void kernel_panic(const char *s) __attribute__((noreturn));

#endif
