#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>
#include <kernel/page_allocator.h>

class Pager {
private:
	struct TableEntry {
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

	struct Table {
		TableEntry pages[1024];
	} __attribute__((packed));

	struct DirectoryEntry {
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

	struct Directory {
		DirectoryEntry tables[1024];
	} __attribute__((packed));
public:
	using TableID = size_t;
	using PageID = size_t;
private:
	PageAllocator *allocator;
	Directory *directory;

	static void LoadDirectory(Directory *);

	Pager(void);
	void MakeTable(TableID);
	void * Map(TableID, PageID, void *);
	void Unmap(TableID);
public:
	static constexpr TableID LOW_MAP        = 1;   // upper bound of 1:1 mapping
	static constexpr TableID KERNEL_RESERVE = 768; // lower bound of kernel

	static Pager * Create();

	void Reload(void);
	void Enable(void);
	void Disable(void);
	bool IsPresent(TableID, PageID);
	void * Alloc(void);
	void * Reserve(void);
	void * AllocIn(TableID, TableID);
	void * AllocAt(TableID, PageID);

	void * AllocAt(void *addr) {
		TableID table = uint32_t(addr) / PAGE_ALLOCATOR_PAGE_SIZE / 1024;
		PageID  page  = uint32_t(addr) / PAGE_ALLOCATOR_PAGE_SIZE % 1024;
		return AllocAt(table, page);
	}
};

#endif
