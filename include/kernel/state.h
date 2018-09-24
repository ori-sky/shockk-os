#ifndef KERNEL_STATE_H
#define KERNEL_STATE_H

#include <kernel/pager.h>
#include <stdint.h>

struct State {
	uint32_t lba;
	Pager *pager;
};

extern State _kernel_state;

#endif
