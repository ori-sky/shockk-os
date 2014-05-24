#ifndef PIC_H
#define PIC_H

#define PIC_PORT_MASTER 0x20
#define PIC_PORT_SLAVE  0xA0
#define PIC_PORT_MASTER_COMMAND PIC_PORT_MASTER
#define PIC_PORT_MASTER_DATA (PIC_PORT_MASTER + 1)
#define PIC_PORT_SLAVE_COMMAND PIC_PORT_SLAVE
#define PIC_PORT_SLAVE_DATA (PIC_PORT_SLAVE + 1)

/* ICW - initialize command word
 *
 * ICW1 - INIT | ICW4
 * ICW2 - vector offset
 * ICW3 - master/slave wired info
 * ICW4 - additional environment info
 */

#define PIC_ICW1_ICW4       0x01 /* ICW4 {not} needed */
#define PIC_ICW1_SINGLE     0x02 /* single {cascade} mode */
#define PIC_ICW1_INTERVAL4  0x04 /* call address interval 4 {8} */
#define PIC_ICW1_LEVEL      0x08 /* level triggered {edge} mode */
#define PIC_ICW1_INIT       0x10 /* initialization */
#define PIC_ICW4_8086       0x01 /* 8086/88 {MCS-80/85} mode */
#define PIC_ICW4_AUTO       0x02 /* auto {normal} end of interrupts */
#define PIC_ICW4_BUF_SLAVE  0x08 /* buffered mode for slave */
#define PIC_ICW4_BUF_MASTER 0x0C /* buffered mode for master */
#define PIC_ICW4_SFNM       0x10 /* {not} special fully nested mode */

extern void pic_set_masks(unsigned char, unsigned char);
extern void pic_remap(unsigned char, unsigned char);

#endif
