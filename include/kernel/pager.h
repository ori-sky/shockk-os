#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>

union PageTableEntry {
	void *address;
	struct {
		uint8_t present       : 1;
		uint8_t writable      : 1;
		uint8_t unprivileged  : 1;
		uint8_t write_through : 1;
		uint8_t disable_cache : 1;
		uint8_t accessed      : 1;
		uint8_t dirty         : 1;
		uint8_t reserved      : 1;
	} __attribute__((packed)) flags;
} __attribute__((packed));

struct PageTable {
	union PageTableEntry pages[1024];
} __attribute__((packed));

union PageDirectoryEntry {
	struct PageTable *address;
	struct {
		uint8_t present       : 1;
		uint8_t writable      : 1;
		uint8_t unprivileged  : 1;
		uint8_t write_through : 1;
		uint8_t disable_cache : 1;
		uint8_t accessed      : 1;
		uint8_t ignored_6     : 1;
		uint8_t page_size     : 1;
	} __attribute__((packed)) flags;
} __attribute__((packed));

struct PageDirectory {
	union PageDirectoryEntry tables[1024];
} __attribute__((packed));

struct Pager {
	struct PageAllocator *allocator;
	struct PageDirectory *directory;
};

void pager_init(struct Pager *, struct PageAllocator *);

#endif
