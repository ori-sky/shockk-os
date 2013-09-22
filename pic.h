#ifndef PIC_H
#define PIC_H

#define PIC_PORT_MASTER      0x20
#define PIC_PORT_SLAVE       0xA0

/* ICW = initialize command word
 *
 * ICW1 = INIT | ICW4
 * ICW2 = vector offset
 * ICW3 = master/slave wired info
 * ICW4 = additional environment info
 */

#define PIC_ICW1_ICW4        0x01 // ICW4 needed
#define PIC_ICW1_SINGLE      0x02 // single mode
#define PIC_ICW1_INTERVAL4   0x04 // call address interval 4
#define PIC_ICW1_LEVEL       0x08 // level triggered mode
#define PIC_ICW1_INIT        0x10 // initialization (required)

#define PIC_ICW4_8086        0x01 // 8086/88 mode
#define PIC_ICW4_AUTO        0x02 // auto end of interrupts
#define PIC_ICW4_BUF_SLAVE   0x08 // buffered mode for slave
#define PIC_ICW4_BUF_MASTER  0x0C // buffered mode for master
#define PIC_ICW4_SFNM        0x10 // special fully nested mode

void pic_remap(unsigned char, unsigned char);

#endif
