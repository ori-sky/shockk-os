#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>
#include <kernel/page_allocator.h>

#define PAGER_LOW_MAP     1 /* number of tables to map 1:1 at lower bound */
#define PAGER_KERNEL    768 /* lower bound of kernel reserve */

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
	PageTableEntry pages[1024];
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
	PageDirectoryEntry tables[1024];
} __attribute__((packed));

class Pager {
public:
	using TableID = size_t;
	using PageID = size_t;
private:
	PageAllocator *allocator;
	PageDirectory *directory;

	Pager(void);
	void MakeTable(TableID);
	void * Map(TableID, PageID, void *);
public:
	static Pager * Create();
	static void LoadDirectory(PageDirectory *);

	void Reload(void);
	void Enable(void);
	void Disable(void);
	void * Alloc(void);
	void * Reserve(void);
	void * AllocIn(TableID, TableID);
	void * AllocAt(TableID, PageID);
};

#endif
