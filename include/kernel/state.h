#ifndef KERNEL_STATE_H
#define KERNEL_STATE_H

#include <kernel/ext2.h>
#include <kernel/maybe.h>
#include <kernel/pager.h>
#include <kernel/task.h>
#include <stdint.h>

struct State {
	Ext2 fs;
	Pager *pager;
	Task *task;
};

extern State _kernel_state;

#endif
