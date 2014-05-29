#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <cpu.h>

#define IRQ0 0x20        /* programmable interval timer */
#define IRQ1 (IRQ0 + 1)  /* keyboard */
#define IRQ2 (IRQ0 + 2)
#define IRQ3 (IRQ0 + 3)
#define IRQ4 (IRQ0 + 4)
#define IRQ5 (IRQ0 + 5)
#define IRQ6 (IRQ0 + 6)
#define IRQ7 (IRQ0 + 7)
#define IRQ8 0x28
#define IRQ9  (IRQ8 + 1)
#define IRQ10 (IRQ8 + 2)
#define IRQ11 (IRQ8 + 3)
#define IRQ12 (IRQ8 + 4)
#define IRQ13 (IRQ8 + 5)
#define IRQ14 (IRQ8 + 6)
#define IRQ15 (IRQ8 + 7)

#define EXC_DIVIDE_BY_ZERO       0x0
#define EXC_DEBUG                0x1
#define EXC_NON_MASK_INTERRUPT   0x2
#define EXC_BREAKPOINT           0x3
#define EXC_OVERFLOW             0x4
#define EXC_BOUND_RANGE_EXCEEDED 0x5
#define EXC_INVALID_OPCODE       0x6
#define EXC_DEV_NOT_AVAILABLE    0x7
#define EXC_DOUBLE_FAULT         0x8
#define EXC_COPROC_SEG_OVERRUN   0x9 /* legacy */
#define EXC_INVALID_TSS          0xA
#define EXC_SEG_NOT_PRESENT      0xB
#define EXC_STACK_SEG_FAULT      0xC
#define EXC_GEN_PROTECT_FAULT    0xD
#define EXC_PAGE_FAULT           0xE
#define EXC_x87_FP_EXCEPTION     0x10
#define EXC_ALIGNMENT_CHECK      0x11
#define EXC_MACHINE_CHECK        0x12
#define EXC_SIMD_FP_EXCEPTION    0x13
#define EXC_VIRT_EXCEPTION       0x14
#define EXC_SECURITY_EXCEPTION   0x1E

extern struct cpu_state * interrupts_handler(struct cpu_state *);

#endif
