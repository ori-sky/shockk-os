#ifndef _SETJMP_H
#define _SETJMP_H 1

typedef void * jmp_buf[4];
//typedef int jmp_buf;

#define setjmp(JB) __builtin_setjmp(JB)

_Noreturn void longjmp(jmp_buf, int);

#endif
