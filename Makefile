export SYSROOT=$(CURDIR)/sysroot
export TOOLCHAIN_PREFIX=$(CURDIR)/toolchain/prefix

export SHK_INCLUDE_DIR=$(CURDIR)/include

export TARGET=i386-shk

export TARGET_ASM=$(TOOLCHAIN_PREFIX)/bin/nasm
export TARGET_CC =$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc
export TARGET_CXX=$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-g++
export TARGET_LD =$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-gcc
export TARGET_AR =$(TOOLCHAIN_PREFIX)/bin/$(TARGET)-ar

export QEMU=qemu-system-i386

export SHKBOOT_BIN=$(CURDIR)/shkboot.bin
export MBR_BIN=$(CURDIR)/mbr.bin
export LOADER_ELF=$(CURDIR)/loader.elf
export KERNEL_ELF=$(CURDIR)/kernel.elf
export CRT0_O=$(CURDIR)/crt0.o
export LIBC_A=$(CURDIR)/libc.a
export LIBM_A=$(CURDIR)/libm.a

export INCLUDE_PATHS=$(CURDIR)/include
export CXXWARNS=-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wstrict-overflow=5 -Wundef -Wdisabled-optimization -Wsign-conversion -Wstack-protector -Winline -Wswitch-enum

export IMAGE=$(CURDIR)/shk.img
IMAGE_MBR=$(CURDIR)/shk-mbr.img
BOOTDIR=$(CURDIR)/boot

.PHONY: all
all: image-mbr

.PHONY: sysroot_headers
sysroot_headers: libc_headers libm_headers

.PHONY: libc_headers
libc_headers: $(SYSROOT)/usr/include/stdio.h

.PHONY: libm_headers
libm_headers: $(SYSROOT)/usr/include/math.h

$(SYSROOT)/usr/include/stdio.h: $(shell find libc/include -type f)
	mkdir -pv $(SYSROOT)/usr/include
	cp -Rv libc/include/* $(dir $@)

$(SYSROOT)/usr/include/math.h: $(shell find libm/include -type f)
	mkdir -pv $(SYSROOT)/usr/include
	cp -Rv libm/include/* $(dir $@)

.PHONY: toolchain
toolchain: libc_headers
	$(MAKE) -C toolchain

.PHONY: $(SHKBOOT_BIN)
$(SHKBOOT_BIN): toolchain
	$(MAKE) -C boot $@

.PHONY: $(MBR_BIN)
$(MBR_BIN): toolchain
	$(MAKE) -C mbr $@

.PHONY: $(LOADER_ELF)
$(LOADER_ELF): toolchain
	$(MAKE) -C loader $@

.PHONY: $(KERNEL_ELF)
$(KERNEL_ELF): toolchain
	$(MAKE) -C kernel $@

.PHONY: $(CRT0_O)
$(CRT0_O): toolchain
	$(MAKE) -C crt0 $@

$(SYSROOT)/usr/lib/crt0.o: $(CRT0_O)
	mkdir -pv $(dir $@)
	cp -v $< $@

.PHONY: $(LIBC_A)
$(LIBC_A): toolchain
	$(MAKE) -C libc $@

$(SYSROOT)/usr/lib/libc.a: $(LIBC_A)
	mkdir -pv $(dir $@)
	cp -v $< $@

.PHONY: $(LIBM_A)
$(LIBM_A): toolchain
	$(MAKE) -C libm $@

$(SYSROOT)/usr/lib/libm.a: $(LIBM_A)
	mkdir -pv $(dir $@)
	cp -v $< $@

.PHONY: crt0
crt0: $(SYSROOT)/usr/lib/crt0.o

.PHONY: libc
libc: $(SYSROOT)/usr/lib/libc.a

.PHONY: libm
libm: $(SYSROOT)/usr/lib/libm.a

.PHONY: test
test: crt0 libc
	$(MAKE) -C test all

.PHONY: usr
usr: crt0 libc libm
	$(MAKE) -C usr all

.PHONY: clean-image
clean-image:
	rm -fv $(IMAGE)

.PHONY: image
image: $(SHKBOOT_BIN) $(LOADER_ELF) $(KERNEL_ELF) test usr
	dd bs=512 count=65536 if=/dev/zero of=$(IMAGE)
	mkfs.ext2 -F $(IMAGE)
	dd bs=512 count=2 if=$(SHKBOOT_BIN) of=$(IMAGE) conv=notrunc
	e2cp -v $(LOADER_ELF) $(IMAGE):/boot/loader.elf
	e2cp -v $(KERNEL_ELF) $(IMAGE):/boot/kernel.elf
	$(MAKE) -C test install
	e2mkdir -v $(IMAGE):/bin
	e2cp -v $(SYSROOT)/bin/* $(IMAGE):/bin

.PHONY: image-mbr
image-mbr: image $(MBR_BIN)
	dd bs=512 count=$$(( 65536 + 2048 )) if=/dev/zero of=$(IMAGE_MBR)
	sh scripts/partition_image.sh
	dd bs=448 count=1 if=$(MBR_BIN) of=$(IMAGE_MBR) conv=notrunc
	dd bs=512 count=65536 if=$(IMAGE) of=$(IMAGE_MBR) conv=notrunc seek=2048

.PHONY: qemu
qemu:
	$(QEMU) $(IMAGE)

.PHONY: clean
clean:
	$(MAKE) -C boot clean
	$(MAKE) -C mbr clean
	$(MAKE) -C loader clean
	$(MAKE) -C kernel clean
	$(MAKE) -C crt0 clean
	$(MAKE) -C libc clean
	$(MAKE) -C libm clean

.PHONY: distclean
distclean: clean
	rm -rfv $(SYSROOT)
	rm -fv $(IMAGE)
	rm -fv $(IMAGE_MBR)

.PHONY: toolchain-clean
toolchain-clean:
	$(MAKE) -C toolchain clean

.PHONY: usr-clean
usr-clean:
	$(MAKE) -C usr clean

.PHONY: toolchain-distclean
toolchain-distclean:
	$(MAKE) -C toolchain distclean

.PHONY: usr-distclean
usr-distclean:
	$(MAKE) -C usr distclean
