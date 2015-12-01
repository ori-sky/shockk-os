IMAGE=os.img
FLOPPY_IMAGE=floppy.img

all: floppy

run-qemu: floppy
	qemu -s -hda $(FLOPPY_IMAGE)

floppy: image
	dd bs=512 count=2820 if=/dev/zero of=$(FLOPPY_IMAGE)
	dd conv=notrunc bs=512 if=$(IMAGE) of=$(FLOPPY_IMAGE)

image: bootsector  kernel
	cat bootsector/bootsector.bin kernel/kernel.bin > $(IMAGE)

.PHONY: bootsector
bootsector:
	cd bootsector && $(MAKE)

.PHONY: kernel
kernel:
	cd kernel && $(MAKE)

.PHONY: clean
clean:
	cd bootsector && $(MAKE) clean
	cd kernel && $(MAKE) clean
	rm -fv os.img
	rm -fv floppy.img
