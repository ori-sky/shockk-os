#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include "tss.h"

struct GDTEntry {
	uint16_t limiter_low;
	uint32_t base_address_low : 24;
	uint8_t type : 4;
	uint8_t code_data : 1;
	uint8_t privilege : 2;
	uint8_t present : 1;
	uint8_t limiter_high : 4;
	uint8_t available : 1;
	uint8_t reserved : 1;
	uint8_t size : 1;
	uint8_t granularity : 1;
	uint8_t base_address_high;
} __attribute__((packed));

struct GDTDescriptor {
	uint16_t limiter;
	uint32_t base_address;
} __attribute__((packed));

struct GDT {
	struct GDTDescriptor descriptor;
	struct GDTEntry entries[6];
} __attribute__((packed));

void gdt_init(volatile struct GDT *, volatile struct TSS *);

#endif
