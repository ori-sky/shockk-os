#include "gdt.h"
#include <stdbool.h>

inline void set_entry(volatile struct GDTEntry *entry, uint32_t base_address, uint32_t limiter, uint8_t access) {
	entry->base_address_low = base_address & 0xFFFF;
	entry->base_address_mid = (base_address >> 16) & 0xFF;
	entry->base_address_high = (base_address >> 24) & 0xFF;
	entry->limiter_low = limiter & 0xFFFF;
	entry->access = access;

	uint8_t flags_and_limiter_high = 0;
	flags_and_limiter_high |= 1 << 7; /* page granularity flag */
	flags_and_limiter_high |= 1 << 6; /* size flag */
	flags_and_limiter_high |= 0 << 5; /* reserved */
	flags_and_limiter_high |= 0 << 4; /* available to system programmers */
	flags_and_limiter_high |= (limiter >> 16) & 0xF;
	entry->flags_and_limiter_high = flags_and_limiter_high;
}

inline uint8_t make_access(bool privileged, bool executable, bool dir_con, bool read_write) {
	uint8_t access = 0;
	access |= 1 << 7;                     /* segment present flag */
	if(!privileged) { access |= 3 << 5; } /* privilege ring level */
	access |= 1 << 4;                     /* code/data flag */
	if(executable) { access |= 1 << 3; } /* executable flag */
	if(dir_con)    { access |= 1 << 2; } /* direction/conforming flag */
	if(read_write) { access |= 1 << 1; } /* read/write flag */
	access |= 1 << 0;                     /* CPU access flag */
	return access;
}

void gdt_init(volatile struct GDT *gdt) {
	gdt->descriptor.limiter = sizeof(gdt->entries);
	gdt->descriptor.base_address = &gdt->entries;

	/* null segment */
	for(unsigned int i = 0; i < sizeof(struct GDTEntry); ++i) {
		((unsigned char *)&gdt->entries[0])[i] = 0;
	}
	set_entry(&gdt->entries[1], 0x0, 0xFFFFF, make_access(true, true, false, true));
	set_entry(&gdt->entries[2], 0x0, 0xFFFFF, make_access(true, false, false, true));
	set_entry(&gdt->entries[3], 0x0, 0xFFFFF, make_access(false, true, false, true));
	set_entry(&gdt->entries[4], 0x0, 0xFFFFF, make_access(false, false, false, true));

	__asm__ __volatile__ ("lgdt (%0)" : : "r" (&gdt->descriptor));
}
