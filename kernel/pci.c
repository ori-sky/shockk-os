#include <kernel/ports.h>
#include <kernel/screen.h>
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
	return pci_config_read(bus, device, function, offset) >> (2 - (offset & 2) << 3) & 0xFFFF;
}

uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	return pci_config_read(bus, device, function, offset) >> (3 - (offset & 3) << 3) & 0xFF;
}

void pci_enumerate_buses(void) {
	/* single PCI host controller */
	if((pci_config_read_byte(0, 0, 0, PCI_OFFSET_HEADER_TYPE) & 0x80) == 0) {
		pci_enumerate_bus(0);
	} else {
		for(uint8_t function = 0; function < 8; ++function) {
			if(pci_config_read_word(0, 0, function, PCI_OFFSET_VENDOR_ID) != 0xFFFF) { break; }
			pci_enumerate_bus(function);
		}
	}
}

void pci_enumerate_bus(uint8_t bus) {
	for(uint8_t device = 0; device < 32; ++device) {
		pci_enumerate_device(bus, device);
	}
}

void pci_enumerate_device(uint8_t bus, uint8_t device) {
	/* if vendor ID is 0xFFFF, device does not exist */
	if(pci_config_read_word(bus, device, 0, PCI_OFFSET_VENDOR_ID) == 0xFFFF) { return; }
	pci_enumerate_function(bus, device, 0);

	/* if device has multiple functions */
	if((pci_config_read_byte(bus, device, 0, PCI_OFFSET_HEADER_TYPE) & 0x80) != 0) {
		for(uint8_t function = 1; function < 8; ++function) {
			/* if vendor ID is 0xFFFF, function does not exist */
			if(pci_config_read_word(bus, device, function, PCI_OFFSET_VENDOR_ID) != 0xFFFF) {
				pci_enumerate_function(bus, device, function);
			}
		}
	}
}

void pci_enumerate_function(uint8_t bus, uint8_t device, uint8_t function) {
	uint8_t class = pci_config_read_byte(bus, device, function, PCI_OFFSET_CLASS);
	uint8_t subclass = pci_config_read_byte(bus, device, function, PCI_OFFSET_SUBCLASS);

	char fn_s[] = "bus = 0x    device = 0x    function = 0x   ";
	uitoa(bus, &fn_s[8], 16);
	uitoa(device, &fn_s[23], 16);
	uitoa(function, &fn_s[40], 16);
	char class_s[] = "class = 0x    subclass = 0x   ";
	uitoa(class, &class_s[10], 16);
	uitoa(subclass, &class_s[27], 16);

	switch(class) {
	case 0x0: /* other */
		switch(subclass) {
		case 0x0:
			screen_print(fn_s);
			screen_print("   ");
			screen_print(class_s);
			screen_put('\n');
			break;
		}
		break;
	case 0x1: /* mass storage controller */
		switch(subclass) {
		case 0x1: /* IDE */
			screen_print("found IDE controller\n");
			break;
		case 0x6: /* serial ATA */
			screen_print("found serial ATA controller\n");
			break;
		}
		break;
	case 0x6: /* bridge */
		switch(subclass) {
		case 0x4: /* PCI-to-PCI */
			pci_enumerate_bus(pci_config_read_byte(bus, device, function, PCI_OFFSET_SECONDARY_BUS));
			break;
		}
		break;
	}
}
