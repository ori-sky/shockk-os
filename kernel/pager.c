#include <kernel/page_allocator.h>
#include <kernel/pager.h>

void pager_init(struct Pager *pager, struct PageAllocator *allocator) {
	pager->allocator = allocator;
	pager->directory = page_allocator_alloc(allocator);

	for(unsigned int table = 0; table < 1024; ++table) {
		pager->directory->tables[table].address = NULL;
		pager->directory->tables[table].flags.writable = 1;
	}
}
