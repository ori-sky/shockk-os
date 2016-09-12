#ifndef PORTS_H
#define PORTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

uint8_t ports_inb(unsigned short);
uint16_t ports_ins(unsigned short);
uint32_t ports_inl(unsigned short);
void ports_str_ins(unsigned short, volatile uint16_t *, size_t);
void ports_outb(unsigned short, uint8_t);
void ports_outs(unsigned short, uint16_t);
void ports_outl(unsigned short, uint32_t);

#ifdef __cplusplus
}
#endif

#endif
