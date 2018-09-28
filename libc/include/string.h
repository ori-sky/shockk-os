#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

void * memcpy(void * restrict, const void * restrict, size_t);
char * memmove(void *, const void *, size_t);
char * strcpy(char * restrict, const char * restrict);
char * strdup(const char *);

int memcmp(const void *, const void *, size_t);
int strcmp(const char *, const char *);

char * strchr(const char *, int);
size_t strcspn(const char *, const char *);
char * strpbrk(const char *, const char *);
size_t strspn(const char *, const char *);
char * strstr(const char *, const char *);
char * strtok(char * restrict, const char * restrict);

void * memset(void *, int, size_t);
char * strerror(int);
size_t strlen(const char *);
char * strsignal(int);

#endif
