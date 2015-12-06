#include <kernel/page_allocator_test.h>

void page_allocator_test(struct PageAllocator *allocator) {
	for(size_t i = 1; i < allocator->count; ++i) {
		if(page_allocator_alloc(allocator) == NULL) { kernel_panic("page_allocator test failed (exhausted)"); }
	}

	if(page_allocator_alloc(allocator) != NULL) { kernel_panic("page_allocator test failed (upper bound)"); }

	page_allocator_free(allocator, (void *)((size_t)allocator->head + PAGE_ALLOCATOR_PAGE_SIZE));
	if(page_allocator_alloc(allocator) == NULL) { kernel_panic("page_allocator test failed (free)"); }

	page_allocator_clear(allocator);
}
