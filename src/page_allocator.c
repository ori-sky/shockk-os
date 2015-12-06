#include <kernel/panic.h>
#include <kernel/page_allocator.h>

void page_allocator_init(struct PageAllocator *allocator, void *head, size_t count, size_t reserve) {
	allocator->head = head;
	allocator->count = count;
	allocator->reserve = reserve;

	page_allocator_clear(allocator);
	page_allocator_alloc_at(allocator, 0);

	if((size_t)allocator >= (size_t)head && (size_t)allocator < (size_t)head + count * PAGE_ALLOCATOR_PAGE_SIZE) {
		size_t page = ((size_t)allocator - (size_t)head) / PAGE_ALLOCATOR_PAGE_SIZE;
		page_allocator_alloc_at(allocator, page);
	}
}

void page_allocator_clear(struct PageAllocator *allocator) {
	for(size_t byte = 0; byte < allocator->count; ++byte) {
		allocator->bitmap[byte] = 0;
	}
}

void * page_allocator_alloc_at(struct PageAllocator *allocator, size_t page) {
	size_t byte = page >> 3;
	uint8_t index = page % 8;
	allocator->bitmap[byte] |= 1 << index;
	return (void *)((size_t)allocator->head + page * PAGE_ALLOCATOR_PAGE_SIZE);
}

void * page_allocator_alloc_in(struct PageAllocator *allocator, size_t lower, size_t upper) {
	/* start at rounded-up byte */
	for(size_t byte = lower / 8; byte < upper; ++byte) {
		if(allocator->bitmap[byte] != 0xFF) {
			uint8_t index = __builtin_ctz((unsigned int)~allocator->bitmap[byte]);
			allocator->bitmap[byte] |= 1 << index;

			size_t page = (byte << 3) + index;
			return (void *)((size_t)allocator->head + page * PAGE_ALLOCATOR_PAGE_SIZE);
		}
	}
	return NULL;
}

void * page_allocator_alloc(struct PageAllocator *allocator) {
	return page_allocator_alloc_in(allocator, allocator->reserve, allocator->count);
}

void * page_allocator_reserve(struct PageAllocator *allocator) {
	return page_allocator_alloc_in(allocator, 0, allocator->reserve);
}

void page_allocator_free(struct PageAllocator *allocator, void *ptr) {
	size_t page = ((size_t)ptr - (size_t)allocator->head) / PAGE_ALLOCATOR_PAGE_SIZE;
	size_t byte = page >> 3;
	uint8_t index = page % 8;
	allocator->bitmap[byte] &= ~(1 << index);
}
