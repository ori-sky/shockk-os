#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>

#define PAGER_DIR_ADDRESS 0x1000000

struct PageTableEntry {
	uint8_t present       : 1;
	uint8_t writable      : 1;
	uint8_t unprivileged  : 1;
	uint8_t write_through : 1;
	uint8_t disable_cache : 1;
	uint8_t accessed      : 1;
	uint8_t dirty         : 1;
	uint8_t reserved      : 1;
	uint8_t ignore_8_11   : 4;
	uint32_t address      : 20;
} __attribute__((packed));

struct PageTable {
	struct PageTableEntry pages[1024];
} __attribute__((packed));

struct PageDirectoryEntry {
	uint8_t present       : 1;
	uint8_t writable      : 1;
	uint8_t unprivileged  : 1;
	uint8_t write_through : 1;
	uint8_t disable_cache : 1;
	uint8_t accessed      : 1;
	uint8_t ignored_6     : 1;
	uint8_t page_size     : 1;
	uint8_t ignore_8_11   : 4;
	uint32_t address      : 20;
} __attribute__((packed));

struct PageDirectory {
	struct PageDirectoryEntry tables[1024];
} __attribute__((packed));

struct Pager {
	struct PageAllocator *allocator;
	struct PageDirectory *directory;
};

void pager_init(struct Pager *, struct PageAllocator *);
void pager_set_directory(struct PageDirectory *directory);
void pager_enable(void);

#endif
