#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>
#include <stddef.h>

#define PAGER_HEAD 0x1000000   /*  16M */
#define PAGER_PAGE_SIZE 0x1000 /*   4K */
#define PAGER_NUM_PAGES 8192   /* x 8K = 32M */

struct Pager {
	uint8_t bitmap[PAGER_NUM_PAGES >> 3];
};

void pager_init(struct Pager *);
void * pager_alloc(struct Pager *);
void pager_free(struct Pager *, void *);
void pager_test(struct Pager *);

#endif
