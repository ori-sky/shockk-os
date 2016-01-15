#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>
#include <kernel/page_allocator.h>

#define PAGER_LOW_MAP     1 /* number of tables to map 1:1 at lower bound */
#define PAGER_RESERVE   766 /* lower bound of reserve 1 */
#define PAGER_KERNEL    768 /* lower bound of reserve 2 */

struct PageTableEntry {
	uint8_t present       : 1;
	uint8_t writable      : 1;
	uint8_t unprivileged  : 1;
	uint8_t write_through : 1;
	uint8_t disable_cache : 1;
	uint8_t accessed      : 1;
	uint8_t dirty         : 1;
	uint8_t reserved      : 1;
	uint8_t ignore_8_11   : 4;
	uint32_t address      : 20;
} __attribute__((packed));

struct PageTable {
	struct PageTableEntry pages[1024];
} __attribute__((packed));

struct PageDirectoryEntry {
	uint8_t present       : 1;
	uint8_t writable      : 1;
	uint8_t unprivileged  : 1;
	uint8_t write_through : 1;
	uint8_t disable_cache : 1;
	uint8_t accessed      : 1;
	uint8_t ignored_6     : 1;
	uint8_t page_size     : 1;
	uint8_t ignore_8_11   : 4;
	uint32_t address      : 20;
} __attribute__((packed));

struct PageDirectory {
	struct PageDirectoryEntry tables[1024];
} __attribute__((packed));

struct Pager {
	struct PageAllocator *allocator;
	struct PageDirectory *directory;
};

struct Pager * pager_init(void);
void * pager_map(struct Pager *, unsigned int, unsigned int, void *);
void * pager_alloc(struct Pager *);
void * pager_reserve(struct Pager *);
void pager_reload(struct Pager *);
void pager_enable(void);
void pager_disable(void);

#endif
