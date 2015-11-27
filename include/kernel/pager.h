#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>

struct PageDirectory {
	uint32_t tables[1024];
} __attribute__((packed));

struct PageTable {
	uint32_t pages[1024];
} __attribute__((packed));

struct Pager {
	struct PageAllocator *allocator;
	struct PageDirectory *directory;
};

void pager_init(struct Pager *, struct PageAllocator *);

#endif
