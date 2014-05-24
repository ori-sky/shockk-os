#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <cpu.h>

#define IDT_TYPE_TASK_32      0x5
#define IDT_TYPE_INTERRUPT_16 0x6
#define IDT_TYPE_TRAP_16      0x7
#define IDT_TYPE_INTERRUPT_32 0xE
#define IDT_TYPE_TRAP_32      0xF

struct idt_entry
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t reserved;
	uint8_t gate_type : 4;
	uint8_t storage : 1;
	uint8_t privilege : 2;
	uint8_t present : 1;
	uint16_t offset_high;
} __attribute__((packed));

struct idt_pointer
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

extern void idt_load(void *);
extern void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t, uint8_t, uint8_t,
                         uint8_t);
extern void idt_init(void);
extern struct cpu_state * idt_isr_callback(struct cpu_state *);

#endif
