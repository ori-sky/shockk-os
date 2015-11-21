#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct GDTEntry {
	uint16_t limiter_low;
	uint16_t base_address_low;
	uint8_t base_address_mid;
	uint8_t access;
	uint8_t flags_and_limiter_high;
	uint8_t base_address_high;
} __attribute__((packed));

struct GDTDescriptor {
	uint16_t limiter;
	uint32_t base_address;
} __attribute__((packed));

struct GDT {
	struct GDTDescriptor descriptor;
	struct GDTEntry entries[5];
} __attribute__((packed));

void gdt_init(volatile struct GDT *);

#endif
