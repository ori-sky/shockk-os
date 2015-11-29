#ifndef PCI_H
#define PCI_H

#include <stdint.h>

#define PCI_PORT_CONFIG_ADDRESS 0xCF8
#define PCI_PORT_CONFIG_DATA    0xCFC

#define PCI_OFFSET_DEVICE_ID     0x00
#define PCI_OFFSET_VENDOR_ID     0x02
#define PCI_OFFSET_CLASS         0x08
#define PCI_OFFSET_SUBCLASS      0x09
#define PCI_OFFSET_HEADER_TYPE   0x0D
#define PCI_OFFSET_SECONDARY_BUS 0x1A

struct PCIIdentifier {
	uint8_t function : 3;
	uint8_t device   : 5;
	uint8_t bus;
} __attribute__((packed));

union PCIConfigAddress {
	uint32_t bits;
	struct {
		uint8_t offset;
		struct PCIIdentifier id;
		uint8_t reserved : 7;
		uint8_t enable   : 1;
	} __attribute__((packed)) fields;
} __attribute__((packed));

struct PCIDeviceInfo {
	struct PCIIdentifier id;
	uint8_t baseclass;
	uint8_t subclass;
};

struct PCIEnumeration {
	uint16_t count;
	struct PCIDeviceInfo info[1024];
};

void pci_enumerate_buses(struct PCIEnumeration *);
void pci_enumerate_bus(struct PCIEnumeration *, uint8_t);
void pci_enumerate_device(struct PCIEnumeration *, uint8_t, uint8_t);
void pci_enumerate_function(struct PCIEnumeration *, struct PCIIdentifier);
uint32_t pci_config_read(struct PCIIdentifier, uint8_t);
uint16_t pci_config_read_word(struct PCIIdentifier, uint8_t);
uint8_t pci_config_read_byte(struct PCIIdentifier, uint8_t);

#endif
