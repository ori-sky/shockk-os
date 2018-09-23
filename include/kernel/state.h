#ifndef KERNEL_STATE_H
#define KERNEL_STATE_H

struct State {
	uint32_t lba;
	Pager *pager;
};

#endif
