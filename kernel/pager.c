#include <kernel/screen.h>
#include <kernel/itoa.h>
#include <kernel/page_allocator.h>
#include <kernel/pager.h>

void pager_init(struct Pager *pager, struct PageAllocator *allocator) {
	pager->allocator = allocator;
	pager->directory = page_allocator_alloc_at(allocator, (void *)PAGER_DIR_ADDRESS);

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

	/* initialize tables */
	for(unsigned int table = 0; table < 4; ++table) {
		struct PageTable *table_addr = page_allocator_alloc_at(allocator, (void *)(PAGER_DIR_ADDRESS + (1 + table) * 0x1000));
		pager->directory->tables[table].present = 1;
		pager->directory->tables[table].address = (uint32_t)table_addr >> 12;

		for(unsigned int page = 0; page < 1024; ++page) {
			void *addr = page_allocator_alloc_at(allocator, (void *)((page + table * 1024) * 0x1000));
			table_addr->pages[page].present = 1;
			table_addr->pages[page].writable = 1;
			table_addr->pages[page].unprivileged = 1;
			table_addr->pages[page].write_through = 0;
			table_addr->pages[page].disable_cache = 0;
			table_addr->pages[page].accessed = 0;
			table_addr->pages[page].dirty = 0;
			table_addr->pages[page].reserved = 0;
			table_addr->pages[page].address = (uint32_t)addr >> 12;
		}
	}

	pager_set_directory(pager->directory);
}

void pager_set_directory(struct PageDirectory *directory) {
	__asm__ ("mov %0, %%cr3" : : "r" (directory));
}

void pager_enable(void) {
	uint32_t cr0;
	__asm__ __volatile__ ("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000; /* set bit 31 (paging bit) in cr0 */
	__asm__ ("mov %0, %%cr0" : : "r" (cr0));
}
