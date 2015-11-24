#include <kernel/ports.h>
#include <kernel/pci.h>

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
	return pci_config_read(bus, device, function, offset) >> (~(offset & 2) << 4) & 0xFFFF;
}

uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	return pci_config_read(bus, device, function, offset) >> (~(offset & 3) << 3) & 0xFF;
}

void pci_check_buses(void) {
	/* single PCI host controller */
	if((pci_config_read_byte(0, 0, 0, PCI_OFFSET_HEADER_TYPE) & 0x80) == 0) {
		pci_check_bus(0);
	} else {
		for(uint8_t function = 0; function < 8; ++function) {
			if(pci_config_read_word(0, 0, function, PCI_OFFSET_VENDOR_ID) != 0xFFFF) { break; }
			pci_check_bus(function);
		}
	}
}

void pci_check_bus(uint8_t bus) {
	for(uint8_t device = 0; device < 32; ++device) {
		pci_check_device(bus, device);
	}
}

void pci_check_device(uint8_t bus, uint8_t device) {
	/* if vendor ID is 0xFFFF, device does not exist */
	if(pci_config_read_word(bus, device, 0, PCI_OFFSET_VENDOR_ID) == 0xFFFF) { return; }
	pci_check_function(bus, device, 0);

	screen_print("found device\n");

	/* if device has multiple functions */
	if((pci_config_read_byte(bus, device, 0, PCI_OFFSET_HEADER_TYPE) & 0x80) != 0) {
		for(uint8_t function = 1; function < 8; ++function) {
			/* if vendor ID is 0xFFFF, function does not exist */
			if(pci_config_read_word(bus, device, function, PCI_OFFSET_VENDOR_ID) != 0xFFFF) {
				pci_check_function(bus, device, function);
			}
		}
	}
}

void pci_check_function(uint8_t bus, uint8_t device, uint8_t function) {
	if(pci_config_read_byte(bus, device, function, PCI_OFFSET_CLASS)    == 0x6 &&
	   pci_config_read_byte(bus, device, function, PCI_OFFSET_SUBCLASS) == 0x4) {
		pci_check_bus(pci_config_read_byte(bus, device, function, PCI_OFFSET_SECONDARY_BUS));
	}
}
