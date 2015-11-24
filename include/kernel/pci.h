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

union PCIConfigAddress {
	uint32_t bits;
	struct {
		uint8_t offset;
		uint8_t function : 3;
		uint8_t device     : 5;
		uint8_t bus;
		uint8_t reserved : 7;
		uint8_t enable   : 1;
	} __attribute__((packed)) fields;
} __attribute__((packed));

struct PCIDevice {
	uint16_t device_id;
	uint16_t vendor_id;
	uint16_t status;
	uint8_t class_code;
	uint8_t subclass_code;
	uint8_t revision_id;
	uint8_t header_type;
	uint32_t base_address_0;
	uint32_t base_address_1;
	uint32_t base_address_2;
	uint32_t base_address_3;
	uint32_t base_address_4;
	uint32_t base_address_5;
};

uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

void pci_check_buses(void);
void pci_check_bus(uint8_t bus);
void pci_check_device(uint8_t bus, uint8_t device);
void pci_check_function(uint8_t bus, uint8_t device, uint8_t function);

#endif
