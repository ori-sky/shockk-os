#ifndef ATA_H
#define ATA_H

#define ATA_PORT_PRIMARY_DATA         0x1F0
#define ATA_PORT_PRIMARY_FEATURE      0x1F1
#define ATA_PORT_PRIMARY_SECTOR_COUNT 0x1F2
#define ATA_PORT_PRIMARY_LBA_LOW      0x1F3
#define ATA_PORT_PRIMARY_LBA_MID      0x1F4
#define ATA_PORT_PRIMARY_LBA_HIGH     0x1F5
#define ATA_PORT_PRIMARY_DRIVE_SELECT 0x1F6
#define ATA_PORT_PRIMARY_STATUS       0x1F7
#define ATA_PORT_PRIMARY_COMMAND      0x1F7
#define ATA_PORT_PRIMARY_ALT_STATUS   0x3F6
#define ATA_PORT_PRIMARY_CONTROL      0x3F6

#define ATA_STATUS_ERROR      (1 << 0)
#define ATA_STATUS_DATA_READY (1 << 3)
#define ATA_STATUS_SERVICE    (1 << 4)
#define ATA_STATUS_FAULT      (1 << 5)
#define ATA_STATUS_READY      (1 << 6)
#define ATA_STATUS_BUSY       (1 << 7)

#define ATA_COMMAND_PIO_READ_RETRY 0x20

void ata_init(void);
void ata_read(uint32_t, uint8_t, volatile void *);

#endif
