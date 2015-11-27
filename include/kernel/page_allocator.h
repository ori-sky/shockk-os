#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>

#define PAGE_ALLOCATOR_HEAD 0x1000000   /*  16M */
#define PAGE_ALLOCATOR_PAGE_SIZE 0x1000 /*   4K */
#define PAGE_ALLOCATOR_NUM_PAGES 8192   /* x 8K = 32M */

struct PageAllocator {
	uint8_t bitmap[PAGE_ALLOCATOR_NUM_PAGES >> 3];
};

void page_allocator_init(struct PageAllocator *);
void * page_allocator_alloc(struct PageAllocator *);
void page_allocator_free(struct PageAllocator *, void *);
void page_allocator_test(struct PageAllocator *);

#endif
