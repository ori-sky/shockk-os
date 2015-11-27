#include <kernel/panic.h>
#include <kernel/page_allocator.h>

void page_allocator_init(struct PageAllocator *page_allocator) {
	for(size_t byte = 0; byte < sizeof(page_allocator->bitmap); ++byte) {
		page_allocator->bitmap[byte] = 0;
	}
}

void * page_allocator_alloc(struct PageAllocator *page_allocator) {
	for(size_t byte = 0; byte < sizeof(page_allocator->bitmap); ++byte) {
		if(page_allocator->bitmap[byte] != 0xFF) {
			uint8_t index = __builtin_ctz((unsigned int)~page_allocator->bitmap[byte]);
			page_allocator->bitmap[byte] |= 1 << index;
			size_t page = (byte << 3) + index;
			return (void *)(PAGE_ALLOCATOR_HEAD + page * PAGE_ALLOCATOR_PAGE_SIZE);
		}
	}
	return NULL;
}

void page_allocator_free(struct PageAllocator *page_allocator, void *ptr) {
	size_t page = ((size_t)ptr - PAGE_ALLOCATOR_HEAD) / PAGE_ALLOCATOR_PAGE_SIZE;
	size_t byte = page >> 3;
	uint8_t index = page % 8;
	page_allocator->bitmap[byte] &= ~(1 << index);
}

void page_allocator_test(struct PageAllocator *page_allocator) {
	for(size_t i = 0; i < PAGE_ALLOCATOR_NUM_PAGES; ++i) {
		if(page_allocator_alloc(page_allocator) == NULL) { kernel_panic("page_allocator test failed (exhausted)"); }
	}

	if(page_allocator_alloc(page_allocator) != NULL) { kernel_panic("page_allocator test failed (upper bound)"); }

	page_allocator_free(page_allocator, (void *)PAGE_ALLOCATOR_HEAD);
	if(page_allocator_alloc(page_allocator) == NULL) { kernel_panic("page_allocator test failed (free)"); }

	page_allocator_init(page_allocator);
}
