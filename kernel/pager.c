#include <kernel/page_allocator.h>
#include <kernel/pager.h>

void pager_init(struct Pager *pager, struct PageAllocator *allocator) {
	pager->allocator = allocator;
	pager->directory = page_allocator_alloc(allocator);
}
