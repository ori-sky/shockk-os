#ifndef PORTS_H
#define PORTS_H

unsigned char ports_inb(unsigned short port);
void ports_outb(unsigned short port, unsigned char data);

#endif
