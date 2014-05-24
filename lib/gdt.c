#include <gdt.h>

static struct gdt_entry entries[GDT_NUM_ENTRIES];
static struct gdt_pointer pointer;

void gdt_set_gate(uint16_t i, uint32_t base, uint32_t limit,
                  uint8_t readwrite, uint8_t expandconform, uint8_t executable,
                  uint8_t codedata, uint8_t privilege, uint8_t present,
                  uint8_t unused, uint8_t granularity)
{
	entries[i].base_low  = base & 0xFFFF;
	entries[i].base_mid  = (base >> 16) & 0xFF;
	entries[i].base_high = (base >> 24) & 0xFF;
	entries[i].limit_low  = limit & 0xFFFF;
	entries[i].limit_high = (limit >> 16) & 0xF;
	entries[i].cpu_access = 0;
	entries[i].readwrite = readwrite;
	entries[i].expandconform = expandconform;
	entries[i].executable = executable;
	entries[i].codedata = codedata;
	entries[i].privilege = privilege;
	entries[i].present = present;
	entries[i].unused = unused;
	entries[i].reserved = 0;
	entries[i].protected_32 = 1;
	entries[i].granularity = granularity;
}

void gdt_init(void)
{
	pointer.limit = sizeof(struct gdt_entry) * GDT_NUM_ENTRIES - 1;
	pointer.base = (uint32_t)&entries;

	gdt_set_gate(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFF, 1, 0, 1, 1, 0, 1, 0, 1);
	gdt_set_gate(2, 0, 0xFFFF, 1, 0, 0, 1, 0, 1, 0, 1);

	gdt_flush((uint32_t)&pointer);
}
