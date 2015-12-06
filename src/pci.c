#include <kernel/ports.h>
#include <kernel/memory.h>
#include <kernel/screen.h>
#include <kernel/pci.h>

void pci_enumerate_buses(struct PCIEnumeration *pci_enum) {
	struct PCIIdentifier id = {0, 0, 0};
	pci_enum->count = 0;

	/* single PCI host controller */
	if((pci_config_read_byte(id, PCI_OFFSET_HEADER_TYPE) & 0x80) == 0) {
		pci_enumerate_bus(pci_enum, 0);
	} else {
		for(id.function = 0; id.function < 8; ++id.function) {
			if(pci_config_read_word(id, PCI_OFFSET_VENDOR_ID) != 0xFFFF) { break; }
			pci_enumerate_bus(pci_enum, id.function);
		}
	}
}

void pci_enumerate_bus(struct PCIEnumeration *pci_enum, uint8_t bus) {
	for(uint8_t device = 0; device < 32; ++device) {
		pci_enumerate_device(pci_enum, bus, device);
	}
}

void pci_enumerate_device(struct PCIEnumeration *pci_enum, uint8_t bus, uint8_t device) {
	struct PCIIdentifier id = {.function = 0, .device = device, .bus = bus};

	/* if vendor ID is 0xFFFF, device does not exist */
	if(pci_config_read_word(id, PCI_OFFSET_VENDOR_ID) == 0xFFFF) { return; }
	pci_enumerate_function(pci_enum, id);

	/* if device has multiple functions */
	if((pci_config_read_byte(id, PCI_OFFSET_HEADER_TYPE) & 0x80) != 0) {
		for(uint8_t function = 1; function < 8; ++function) {
			struct PCIIdentifier func_id = {.function = function, .device = device, .bus = bus};

			/* if vendor ID is 0xFFFF, function does not exist */
			if(pci_config_read_word(func_id, PCI_OFFSET_VENDOR_ID) != 0xFFFF) {
				pci_enumerate_function(pci_enum, func_id);
			}
		}
	}
}

void pci_enumerate_function(struct PCIEnumeration *pci_enum, struct PCIIdentifier id) {
	uint8_t baseclass = pci_config_read_byte(id, PCI_OFFSET_CLASS);
	uint8_t subclass = pci_config_read_byte(id, PCI_OFFSET_SUBCLASS);

	pci_enum->info[pci_enum->count].id = id;
	pci_enum->info[pci_enum->count].baseclass = baseclass;
	pci_enum->info[pci_enum->count].subclass = subclass;
	++pci_enum->count;

	switch(baseclass) {
	case 0x6: /* bridge */
		switch(subclass) {
		case 0x4: /* PCI-to-PCI */
			pci_enumerate_bus(pci_enum, pci_config_read_byte(id, PCI_OFFSET_SECONDARY_BUS));
			break;
		}
		break;
	}
}

uint32_t pci_config_read(struct PCIIdentifier id, uint8_t offset) {
	union PCIConfigAddress addr;
	addr.fields.offset = offset & ~3; /* offset must be aligned to 32-bit boundary so bits 0 and 1 must be cleared */
	addr.fields.id = id;
	addr.fields.reserved = 0;
	addr.fields.enable = 1;
	ports_outl(PCI_PORT_CONFIG_ADDRESS, addr.bits);
	return ports_inl(PCI_PORT_CONFIG_DATA);
}

uint16_t pci_config_read_word(struct PCIIdentifier id, uint8_t offset) {
	return pci_config_read(id, offset) >> ((2 - (offset & 2)) << 3) & 0xFFFF;
}

uint8_t pci_config_read_byte(struct PCIIdentifier id, uint8_t offset) {
	return pci_config_read(id, offset) >> ((3 - (offset & 3)) << 3) & 0xFF;
}
