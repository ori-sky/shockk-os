#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_NUM_ENTRIES 3

struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t cpu_access : 1;
	uint8_t readwrite : 1;
	uint8_t expandconform : 1;
	uint8_t executable : 1;
	uint8_t codedata : 1;
	uint8_t privilege : 2;
	uint8_t present : 1;
	uint8_t limit_high : 4;
	uint8_t unused : 1;
	uint8_t reserved : 1;
	uint8_t protected_32 : 1;
	uint8_t granularity : 1;
	uint8_t base_high;
} __attribute__((packed));

struct gdt_pointer
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

extern void gdt_set_gate(uint16_t, uint32_t, uint32_t, uint8_t, uint8_t, uint8_t,
                         uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
extern void gdt_init(void);

#endif
