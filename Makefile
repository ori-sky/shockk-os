IMAGE=os.img
FLOPPY_IMAGE=floppy.img

all: floppy

run-qemu: floppy
	qemu -s -hda $(FLOPPY_IMAGE)

floppy: image
	dd bs=512 count=2820 if=/dev/zero of=$(FLOPPY_IMAGE)
	dd conv=notrunc bs=512 if=$(IMAGE) of=$(FLOPPY_IMAGE)

image: bootloader  kernel
	cat bootloader/bootloader.bin kernel/kernel.bin > $(IMAGE)

.PHONY: bootloader
bootloader:
	cd bootloader && $(MAKE)

.PHONY: kernel
kernel:
	cd kernel && $(MAKE)

.PHONY: clean
clean:
	cd bootloader && $(MAKE) clean
	cd kernel && $(MAKE) clean
	rm -fv os.img
	rm -fv floppy.img
