#ifndef _MALLOC_H
#define _MALLOC_H

struct mallinfo {
};

struct mallinfo mallinfo(void);
int mallopt(int, int);

#endif
