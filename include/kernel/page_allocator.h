#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>

#define PAGE_ALLOCATOR_HEAD 0x0
#define PAGE_ALLOCATOR_PAGE_SIZE 0x1000 /*    4K       */
#define PAGE_ALLOCATOR_NUM_PAGES 0x4000 /* x 16K = 64M */
#define PAGE_ALLOCATOR_PTR_ERROR ((void *)0x1)

struct PageAllocator {
	uint8_t bitmap[PAGE_ALLOCATOR_NUM_PAGES >> 3];
};

void page_allocator_init(struct PageAllocator *);
void * page_allocator_alloc(struct PageAllocator *);
void * page_allocator_alloc_at(struct PageAllocator *, void *);
void page_allocator_free(struct PageAllocator *, void *);
void page_allocator_test(struct PageAllocator *);

#endif
