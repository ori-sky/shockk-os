#include <kernel/pager.h>

static void pager_make_table(struct Pager *pager, unsigned int table) {
	struct PageTable *table_addr = page_allocator_reserve(pager->allocator);
	pager->directory->tables[table].present = 1;
	pager->directory->tables[table].address = (uint32_t)table_addr >> 12;
}

static void pager_load_directory(struct PageDirectory *directory) {
	__asm__ ("mov %0, %%cr3" : : "r" (directory));
}

struct Pager * pager_init(void) {
	struct PageAllocator *allocator = (struct PageAllocator *)0x100000;
	page_allocator_init(allocator, NULL, 4096, PAGER_LOW_MAP * 1024);

	/* reserve physical NULL to 0x10000 */
	for(size_t page = 0; page < 0x10; ++page) {
		page_allocator_alloc_at(allocator, page);
	}

	/* reserve physical 0x50000 to upper bound of 1:1 mapping */
	for(size_t page = 0x50; page < PAGER_LOW_MAP * 1024; ++page) {
		page_allocator_alloc_at(allocator, page);
	}

	struct Pager *pager = page_allocator_reserve(allocator);
	pager->allocator = allocator;
	pager->directory = page_allocator_reserve(allocator);

	/* initialize directory */
	for(unsigned int table = 0; table < 1024; ++table) {
		pager->directory->tables[table].present = 0;
		pager->directory->tables[table].writable = 1;
		pager->directory->tables[table].unprivileged = 0;
		pager->directory->tables[table].write_through = 0;
		pager->directory->tables[table].disable_cache = 0;
		pager->directory->tables[table].accessed = 0;
		pager->directory->tables[table].page_size = 0;
	}

	/* initialize 1:1 mapping */
	for(unsigned int table = 0; table < PAGER_LOW_MAP; ++table) {
		for(unsigned int page = 0; page < 1024; ++page) {
			pager_map(pager, table, page, (void *)((table * 1024 + page) * PAGE_ALLOCATOR_PAGE_SIZE));
		}
	}

	/* reserve page for pager itself */
	return pager_map(pager, (uintptr_t)pager / 4096 / 1024, (uintptr_t)pager / 4096 % 1024, pager);
}

void * pager_map(struct Pager *pager, unsigned int table, unsigned int page, void *phys_addr) {
	if(!pager->directory->tables[table].present) { pager_make_table(pager, table); }

	struct PageTable *table_addr = (struct PageTable *)(pager->directory->tables[table].address << 12);
	table_addr->pages[page].present = 1;
	table_addr->pages[page].writable = 1;
	table_addr->pages[page].unprivileged = 1;
	table_addr->pages[page].write_through = 0;
	table_addr->pages[page].disable_cache = 0;
	table_addr->pages[page].accessed = 0;
	table_addr->pages[page].dirty = 0;
	table_addr->pages[page].reserved = 0;
	table_addr->pages[page].address = (uint32_t)phys_addr >> 12;
	return (void *)((table * 1024 + page) * PAGE_ALLOCATOR_PAGE_SIZE);
}

static void * pager_alloc_at(struct Pager *pager, unsigned int table, unsigned int page) {
	return pager_map(pager, table, page, page_allocator_alloc(pager->allocator));
}

static void * pager_alloc_in(struct Pager *pager, unsigned int lower, unsigned int upper) {
	/* lower bound has to be above low 1:1 mapping */
	if(lower < PAGER_LOW_MAP) { return NULL; }

	char s1[] = "                                ";
	itoa((int)pager->directory, s1, 16);
	//if(lower == PAGER_RESERVE_2) { kernel_panic(s1); }

	/* attempt to allocate from existing page table */
	for(unsigned int table = lower; table < upper; ++table) {
		if(pager->directory->tables[table].present) {
			for(unsigned int page = 0; page < 1024; ++page) {
				struct PageTable *table_addr = (struct PageTable *)(pager->directory->tables[table].address << 12);
				if(!table_addr->pages[page].present) {
					return pager_alloc_at(pager, table, page);
				}
			}
		}
	}

	/* attempt to allocate from new page table */
	for(unsigned int table = lower; table < upper; ++table) {
		if(!pager->directory->tables[table].present) {
			return pager_alloc_at(pager, table, 0);
		}
	}

	/* otherwise fail */
	return NULL;
}

void * pager_alloc(struct Pager *pager) {
	return pager_alloc_in(pager, PAGER_LOW_MAP, PAGER_RESERVE_2);
}

void * pager_reserve(struct Pager *pager) {
	return pager_alloc_in(pager, PAGER_RESERVE_2, 1024);
}

void pager_reload(struct Pager *pager) {
	pager_load_directory(pager->directory);
}

void pager_enable(void) {
	uint32_t cr0;
	__asm__ __volatile__ ("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000; /* set bit 31 (paging bit) in cr0 */
	__asm__ ("mov %0, %%cr0" : : "r" (cr0));
}

void pager_disable(void) {
	uint32_t cr0;
	__asm__ __volatile__ ("mov %%cr0, %0" : "=r" (cr0));
	cr0 &= ~0x80000000; /* clear bit 31 (paging bit) in cr0 */
	__asm__ ("mov %0, %%cr0" : : "r" (cr0));
}
