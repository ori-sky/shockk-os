#ifndef KERNEL_STATE_H
#define KERNEL_STATE_H

#include <kernel/ext2.h>
#include <kernel/maybe.h>
#include <kernel/pager.h>
#include <kernel/screen.h>
#include <kernel/task.h>
#include <kernel/tss.h>
#include <stdint.h>

struct State {
	Screen screen;
	Ext2 fs;
	Pager *pager;
	TSS *tss;
	Task *task;
	uint32_t next_pid;
	Task *pids[16];
};

extern State _kernel_state;

#endif
