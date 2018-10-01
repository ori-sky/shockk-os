#ifndef KERNEL_STATE_H
#define KERNEL_STATE_H

#include <kernel/maybe.h>
#include <kernel/pager.h>
#include <kernel/task.h>
#include <stdint.h>

struct State {
	uint32_t lba;
	Pager *pager;
	Task *next;
};

extern State _kernel_state;

#endif
