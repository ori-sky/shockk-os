#include <stdbool.h>
#include <kernel/gdt.h>

inline void set_entry_common(volatile struct GDTEntry *entry, uint32_t base_address, uint32_t limiter, bool privileged) {
	entry->base_address_low = base_address & 0xFFFFFF;
	entry->base_address_high = (base_address >> 24) & 0xFF;
	entry->limiter_low = limiter & 0xFFFF;
	entry->limiter_high = (limiter >> 16) & 0xF;
	entry->present = 1;
	entry->privilege = privileged ? 0 : 3;
	entry->granularity = 1;
	entry->size = 1;
	entry->reserved = 0;
	entry->available = 0;
}

inline void set_entry(volatile struct GDTEntry *entry, uint32_t base_address, uint32_t limiter, bool privileged, bool executable) {
	set_entry_common(entry, base_address, limiter, privileged);
	entry->code_data = 1;
	entry->type = 2 | (executable ? 8 : 0); /* read/write flag OR'd with executable flag */
}

inline void set_tss_entry(volatile struct GDTEntry *entry, uint32_t base_address, uint32_t limiter, bool privileged) {
	set_entry_common(entry, base_address, limiter, privileged);
	entry->code_data = 0;
	entry->type = 1 | 8; /* TSS type with bit 1 (busy flag) cleared */
}

void gdt_init(volatile struct GDT *gdt, volatile struct TSS *tss) {
	gdt->descriptor.limiter = sizeof(gdt->entries);
	gdt->descriptor.base_address = &gdt->entries;

	set_entry(&gdt->entries[1], 0x0, 0xFFFFF, true, true);
	set_entry(&gdt->entries[2], 0x0, 0xFFFFF, true, false);
	set_entry(&gdt->entries[3], 0x0, 0xFFFFF, false, true);
	set_entry(&gdt->entries[4], 0x0, 0xFFFFF, false, false);
	set_tss_entry(&gdt->entries[5], tss, sizeof(struct TSS), true);

	__asm__ ("lgdt (%0)" : : "r" (&gdt->descriptor));
}
