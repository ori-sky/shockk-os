#include <kernel/panic.h>
#include <kernel/pager.h>

void pager_init(struct Pager *pager) {
	for(size_t byte = 0; byte < sizeof(pager->bitmap); ++byte) {
		pager->bitmap[byte] = 0;
	}
}

void * pager_alloc(struct Pager *pager) {
	for(size_t byte = 0; byte < sizeof(pager->bitmap); ++byte) {
		if(pager->bitmap[byte] != 0xFF) {
			uint8_t index = __builtin_ctz(~pager->bitmap[byte]);
			pager->bitmap[byte] |= 1 << index;
			size_t page = (byte << 3) + index;
			return (void *)(PAGER_HEAD + page * PAGER_PAGE_SIZE);
		}
	}
	return NULL;
}

void pager_free(struct Pager *pager, void *ptr) {
	size_t page = ((size_t)ptr - PAGER_HEAD) / PAGER_PAGE_SIZE;
	size_t byte = page >> 3;
	uint8_t index = page % 8;
	pager->bitmap[byte] &= ~(1 << index);
}

void pager_test(struct Pager *pager) {
	for(size_t i = 0; i < PAGER_NUM_PAGES; ++i) {
		if(pager_alloc(pager) == NULL) { kernel_panic("pager test failed (exhausted)"); }
	}

	if(pager_alloc(pager) != NULL) { kernel_panic("pager test failed (upper bound)"); }

	pager_free(pager, (void *)PAGER_HEAD);
	if(pager_alloc(pager) == NULL) { kernel_panic("pager test failed (free)"); }

	pager_init(pager);
}
