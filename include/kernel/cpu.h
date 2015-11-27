#ifndef CPU_H
#define CPU_H

#include <stdint.h>

struct CPUState {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t interrupt;
	uint32_t error;

	/* pushed automatically by CPU */
	uint32_t iret_eip;
	uint32_t iret_cs;
	uint32_t iret_eflags;
	uint32_t iret_esp;
	uint32_t iret_ss;
};

#endif
