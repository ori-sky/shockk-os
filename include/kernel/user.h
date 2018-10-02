#ifndef KERNEL_USER_H
#define KERNEL_USER_H

extern "C" void user_enter(void (*entry)(), void *stack) __attribute__((noreturn));

#endif
