#include <kernel/ports.h>
#include <kernel/memory.h>
#include <kernel/screen.h>
#include <kernel/pci.h>

void pci_enumerate_buses(struct PCIEnumeration *pci_enum) {
	/* single PCI host controller */
	if((pci_config_read_byte(0, 0, 0, PCI_OFFSET_HEADER_TYPE) & 0x80) == 0) {
		pci_enumerate_bus(pci_enum, 0);
	} else {
		for(uint8_t function = 0; function < 8; ++function) {
			if(pci_config_read_word(0, 0, function, PCI_OFFSET_VENDOR_ID) != 0xFFFF) { break; }
			pci_enumerate_bus(pci_enum, function);
		}
	}
}

void pci_enumerate_bus(struct PCIEnumeration *pci_enum, uint8_t bus) {
	for(uint8_t device = 0; device < 32; ++device) {
		pci_enumerate_device(pci_enum, bus, device);
	}
}

void pci_enumerate_device(struct PCIEnumeration *pci_enum, uint8_t bus, uint8_t device) {
	/* if vendor ID is 0xFFFF, device does not exist */
	if(pci_config_read_word(bus, device, 0, PCI_OFFSET_VENDOR_ID) == 0xFFFF) { return; }
	pci_enumerate_function(pci_enum, bus, device, 0);

	/* if device has multiple functions */
	if((pci_config_read_byte(bus, device, 0, PCI_OFFSET_HEADER_TYPE) & 0x80) != 0) {
		for(uint8_t function = 1; function < 8; ++function) {
			/* if vendor ID is 0xFFFF, function does not exist */
			if(pci_config_read_word(bus, device, function, PCI_OFFSET_VENDOR_ID) != 0xFFFF) {
				pci_enumerate_function(pci_enum, bus, device, function);
			}
		}
	}
}

void pci_enumerate_function(struct PCIEnumeration *pci_enum, uint8_t bus, uint8_t device, uint8_t function) {
	uint8_t baseclass = pci_config_read_byte(bus, device, function, PCI_OFFSET_CLASS);
	uint8_t subclass = pci_config_read_byte(bus, device, function, PCI_OFFSET_SUBCLASS);

	pci_enum->identifiers[pci_enum->count].bus = bus;
	pci_enum->identifiers[pci_enum->count].device = device;
	pci_enum->identifiers[pci_enum->count].function = function;
	pci_enum->identifiers[pci_enum->count].baseclass = baseclass;
	pci_enum->identifiers[pci_enum->count].subclass = subclass;
	++pci_enum->count;

	switch(baseclass) {
	case 0x6: /* bridge */
		switch(subclass) {
		case 0x4: /* PCI-to-PCI */
			pci_enumerate_bus(pci_enum, pci_config_read_byte(bus, device, function, PCI_OFFSET_SECONDARY_BUS));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	union PCIConfigAddress addr;
	addr.fields.offset = offset & ~3; /* offset must be aligned to 32-bit boundary so bits 0 and 1 must be cleared */
	addr.fields.function = function;
	addr.fields.device = device;
	addr.fields.bus = bus;
	addr.fields.reserved = 0;
	addr.fields.enable = 1;
	ports_outl(PCI_PORT_CONFIG_ADDRESS, addr.bits);
	return ports_inl(PCI_PORT_CONFIG_DATA);
}

uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	return pci_config_read(bus, device, function, offset) >> ((2 - (offset & 2)) << 3) & 0xFFFF;
}

uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	return pci_config_read(bus, device, function, offset) >> ((3 - (offset & 3)) << 3) & 0xFF;
}
