#include <kernel/mbr.h>
#include <kernel/ata.h>

MBR mbr_read(void) {
	MBR mbr;
	ata_pio_read(0, 1, &mbr);
	return mbr;
}
