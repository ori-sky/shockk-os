#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>

#define PAGE_ALLOCATOR_PAGE_SIZE 0x1000 /* 4K */

struct PageAllocator {
	void *head;
	size_t count;
	size_t reserve;
	uint8_t *bitmap;
};

void page_allocator_init(struct PageAllocator *, void *, size_t, size_t);
void page_allocator_alloc_self(struct PageAllocator *allocator);
void page_allocator_clear(struct PageAllocator *);
void * page_allocator_alloc_at(struct PageAllocator *, size_t);
void * page_allocator_alloc_in(struct PageAllocator *, size_t, size_t);
void * page_allocator_alloc(struct PageAllocator *);
void * page_allocator_reserve(struct PageAllocator *);
void page_allocator_free(struct PageAllocator *, void *);

#endif
