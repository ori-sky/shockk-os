#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

uint8_t ports_inb(unsigned short port);
uint16_t ports_ins(unsigned short port);
uint32_t ports_inl(unsigned short port);
void ports_outb(unsigned short port, uint8_t data);
void ports_outs(unsigned short port, uint16_t data);
void ports_outl(unsigned short port, uint32_t data);

#endif
