#include <kernel/pager.h>

Pager * Pager::Create(void) {
	struct PageAllocator *allocator = (struct PageAllocator *)0x100000;
	allocator->bitmap = (uint8_t *)0x200000;
	page_allocator_init(allocator, NULL, 4096, LOW_MAP * 1024);

	/* reserve physical range NULL to 0x10000 */
	for(size_t page = 0; page < 0x10; ++page) {
		page_allocator_alloc_at(allocator, page);
	}

	page_allocator_alloc_self(allocator);

	/* reserve physical range 0x50000 to upper bound of 1:1 mapping */
	for(size_t page = 0x50; page < LOW_MAP * 1024; ++page) {
		page_allocator_alloc_at(allocator, page);
	}

	Pager *pager = static_cast<Pager *>(page_allocator_reserve(allocator));
	pager->allocator = allocator;
	pager->directory = static_cast<Directory *>(page_allocator_reserve(allocator));

	/* initialize directory */
	for(unsigned int table = 0; table < 1024; ++table) {
		pager->directory->tables[table].present = 0;
		pager->directory->tables[table].writable = 1;
		pager->directory->tables[table].unprivileged = 1;
		pager->directory->tables[table].write_through = 0;
		pager->directory->tables[table].disable_cache = 0;
		pager->directory->tables[table].accessed = 0;
		pager->directory->tables[table].page_size = 0;
	}

	/* initialize 1:1 mapping */
	for(unsigned int table = 0; table < LOW_MAP; ++table) {
		for(unsigned int page = 0; page < 1024; ++page) {
			pager->Map(table, page, (void *)((table * 1024 + page) * PAGE_ALLOCATOR_PAGE_SIZE));
		}
	}

	/* reserve page for pager itself */
	return static_cast<Pager *>(
		pager->Map((uintptr_t)pager / 4096 / 1024, (uintptr_t)pager / 4096 % 1024, pager)
	);
}

void Pager::LoadDirectory(Directory *dir) {
	__asm__ ("mov %0, %%cr3" : : "r" (dir));
}

void Pager::Reload(void) {
	LoadDirectory(this->directory);
}

void Pager::Enable(void) {
	uint32_t cr0;
	__asm__ __volatile__ ("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000; /* set bit 31 (paging bit) in cr0 */
	__asm__ ("mov %0, %%cr0" : : "r" (cr0));
}

void Pager::Disable(void) {
	uint32_t cr0;
	__asm__ __volatile__ ("mov %%cr0, %0" : "=r" (cr0));
	cr0 &= ~0x80000000; /* clear bit 31 (paging bit) in cr0 */
	__asm__ ("mov %0, %%cr0" : : "r" (cr0));
}

bool Pager::IsPresent(TableID table, PageID page) {
	if(!this->directory->tables[table].present) { return false; }
	Table *table_addr = (Table *)(this->directory->tables[table].address << 12);
	return table_addr->pages[page].present == 1;
}

void * Pager::Alloc(void) {
	return AllocIn(LOW_MAP, KERNEL_RESERVE);
}

void * Pager::Reserve(void) {
	return AllocIn(KERNEL_RESERVE, 1024);
}

void * Pager::AllocIn(TableID lower, TableID upper) {
	/* lower bound has to be above low 1:1 mapping */
	if(lower < LOW_MAP) { return NULL; }

	/* attempt to allocate from existing page table */
	for(unsigned int table = lower; table < upper; ++table) {
		if(this->directory->tables[table].present) {
			for(unsigned int page = 0; page < 1024; ++page) {
				Table *table_addr = (Table *)(this->directory->tables[table].address << 12);
				if(!table_addr->pages[page].present) {
					return AllocAt(table, page);
				}
			}
		}
	}

	/* attempt to allocate from new page table */
	for(unsigned int table = lower; table < upper; ++table) {
		if(!this->directory->tables[table].present) {
			return AllocAt(table, 0);
		}
	}

	/* otherwise fail */
	return NULL;
}

void * Pager::AllocAt(TableID table, PageID page) {
	return Map(table, page, page_allocator_alloc(this->allocator));
}

void Pager::MakeTable(TableID table) {
	Table *table_addr = static_cast<Table *>(page_allocator_reserve(this->allocator));
	this->directory->tables[table].present = 1;
	this->directory->tables[table].address = (uint32_t)table_addr >> 12;
}

void * Pager::Map(TableID table, PageID page, void *phys_addr) {
	if(!this->directory->tables[table].present) { MakeTable(table); }

	Table *table_addr = (Table *)(this->directory->tables[table].address << 12);
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
