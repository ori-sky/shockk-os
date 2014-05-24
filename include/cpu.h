#ifndef CPU_H
#define CPU_H

struct cpu_state
{
	uint32_t ds;

	uint32_t edi;
	uint32_t esi;
	uint8_t *ebp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	uint32_t interrupt;
	uint32_t error;

	void *eip;
	uint32_t cs;
	uint32_t eflags;
	void *esp;
	uint32_t ss;
} __attribute__((packed));

#endif
