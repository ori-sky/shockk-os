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

	class Context {
	private:
		Pager *parent;
		Directory *directory;

		void Make(TableID, Table *, bool = true);
		void Make(TableID, bool = true);
	public:
		Context(Pager *, Directory *, bool = true);

		const Directory * GetDirectory(void) { return directory; }

		bool IsPresent(TableID, PageID);

		void * Alloc(void) { return AllocIn(LOW_MAP, KERNEL_RESERVE); }
		void * Reserve(void) { return AllocIn(KERNEL_RESERVE, 1024); }
		void * AllocAt(void *addr) {
			TableID table = uint32_t(addr) / PAGE_ALLOCATOR_PAGE_SIZE / 1024;
			PageID  page  = uint32_t(addr) / PAGE_ALLOCATOR_PAGE_SIZE % 1024;
			return AllocAt(table, page);
		}
		void * AllocAt(TableID, PageID);
		void * AllocIn(TableID, TableID);

		void * Map(TableID, PageID, void *);
		void Unmap(TableID);
	};
private:
	PageAllocator *allocator;
	Context context;

	static void Load(const Directory *);

	Pager(void);

	void Reload(void) { Load(context.GetDirectory()); }
public:
	static constexpr TableID LOW_MAP        = 1;   // upper bound of 1:1 mapping
	static constexpr TableID KERNEL_RESERVE = 768; // lower bound of kernel

	static Pager * Create();

	Context & GetContext(void) { return context; }
	Context MakeContext(bool = true);

	void Load(Context);
	void Enable(Context);
	void Enable(void);
	void Disable(void);
};

#endif
