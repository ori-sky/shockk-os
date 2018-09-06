NASM_VERSION=2.12.02
BINUTILS_VERSION=2.31
GCC_VERSION=8.2.0
NEWLIB_VERSION=3.0.0

.PHONY: all
all: nasm binutils gcc newlib

.PHONY: nasm
nasm: $(TOOLCHAIN_HOSTED)/bin/nasm

.PHONY: binutils-bare
binutils-bare: $(TOOLCHAIN_BARE)/bin/$(TARGET)-ar

.PHONY: gcc-bare
gcc-bare: $(TOOLCHAIN_BARE)/bin/$(TARGET)-gcc

.PHONY: newlib-bare
newlib-bare: $(TOOLCHAIN_SYSROOT)/usr/lib/libc.a

.PHONY: binutils
binutils: $(TOOLCHAIN_HOSTED)/bin/$(TARGET)-ar

.PHONY: gcc
gcc: $(TOOLCHAIN_HOSTED)/bin/$(TARGET)-gcc

# XXX: rebuild newlib with hosted toolchain
.PHONY: newlib
newlib: $(TOOLCHAIN_SYSROOT)/usr/lib/libc.a

# downloads

nasm-$(NASM_VERSION).tar.gz:
	wget http://www.nasm.us/pub/nasm/releasebuilds/$(NASM_VERSION)/nasm-$(NASM_VERSION).tar.gz

binutils-$(BINUTILS_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.gz

gcc-$(GCC_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.gz

newlib-$(NEWLIB_VERSION).tar.gz:
	wget ftp://sourceware.org/pub/newlib/newlib-$(NEWLIB_VERSION).tar.gz

# patches

nasm-$(NASM_VERSION): nasm-$(NASM_VERSION).tar.gz
	tar -xvf $<

binutils-$(BINUTILS_VERSION): binutils-$(BINUTILS_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-binutils-$(BINUTILS_VERSION).patch

gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-gcc-$(GCC_VERSION).patch

newlib-$(NEWLIB_VERSION): newlib-$(NEWLIB_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-newlib-$(NEWLIB_VERSION).patch

# newlib syscalls.c and crt0.c from shk

newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/syscalls.c: $(SHK_LIBC_SOURCE_DIR)/syscalls.c
	cp -v $^ $@

newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/crt0.c: $(SHK_LIBC_SOURCE_DIR)/crt0.c
	cp -v $^ $@

# gcc prerequisites

gcc-$(GCC_VERSION)/gmp: gcc-$(GCC_VERSION)
	cd gcc-$(GCC_VERSION) && sh contrib/download_prerequisites
	touch $@

# nasm build

$(TOOLCHAIN_HOSTED)/bin/nasm: nasm-$(NASM_VERSION)
	mkdir -pv build-nasm-$(NASM_VERSION)
	cd build-nasm-$(NASM_VERSION) && ../$</configure --prefix="$(TOOLCHAIN_HOSTED)"
	$(MAKE) -C build-nasm-$(NASM_VERSION)
	$(MAKE) -C build-nasm-$(NASM_VERSION) install

# bare binutils build

$(TOOLCHAIN_BARE)/bin/$(TARGET)-ar: binutils-$(BINUTILS_VERSION)
	mkdir -pv build-binutils-$(BINUTILS_VERSION)-bare
	cd build-binutils-$(BINUTILS_VERSION)-bare && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_BARE)" --with-sysroot --disable-nls --disable-werror
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION)-bare
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION)-bare install

# bare gcc build

$(TOOLCHAIN_BARE)/bin/$(TARGET)-gcc: gcc-$(GCC_VERSION)/gmp $(TOOLCHAIN_BARE)/bin/$(TARGET)-ar
	mkdir -pv build-gcc-$(GCC_VERSION)-bare
	cd build-gcc-$(GCC_VERSION)-bare && ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --prefix="$(TOOLCHAIN_BARE)" --disable-nls --enable-languages=c,c++ --without-headers
	$(MAKE) -C build-gcc-$(GCC_VERSION)-bare all-gcc
	$(MAKE) -C build-gcc-$(GCC_VERSION)-bare all-target-libgcc
	$(MAKE) -C build-gcc-$(GCC_VERSION)-bare install-gcc
	$(MAKE) -C build-gcc-$(GCC_VERSION)-bare install-target-libgcc

# bare newlib build

build-newlib-$(NEWLIB_VERSION)-bare: $(TOOLCHAIN_BARE)/bin/$(TARGET)-ar $(TOOLCHAIN_BARE)/bin/$(TARGET)-gcc
	mkdir -pv build-newlib-$(NEWLIB_VERSION)-bare
	export PATH="$(TOOLCHAIN_BARE)/bin:$$PATH"; cd build-newlib-$(NEWLIB_VERSION)-bare && ../newlib-$(NEWLIB_VERSION)/configure --target=$(TARGET) --prefix=/usr

$(TOOLCHAIN_SYSROOT)/usr/lib/libc.a: build-newlib-$(NEWLIB_VERSION)-bare newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/syscalls.c newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/crt0.c
	export PATH="$(TOOLCHAIN_BARE)/bin:$$PATH"; $(MAKE) -C build-newlib-$(NEWLIB_VERSION)-bare all
	export PATH="$(TOOLCHAIN_BARE)/bin:$$PATH"; $(MAKE) -C build-newlib-$(NEWLIB_VERSION)-bare DESTDIR=$(TOOLCHAIN_SYSROOT) install
	rm -rfv "$(TOOLCHAIN_SYSROOT)/usr/include" "$(TOOLCHAIN_SYSROOT)/usr/lib"
	mv -v "$(TOOLCHAIN_SYSROOT)/usr/$(TARGET)/include" "$(TOOLCHAIN_SYSROOT)/usr/$(TARGET)/lib" "$(TOOLCHAIN_SYSROOT)/usr"
	rmdir -v $(TOOLCHAIN_SYSROOT)/usr/$(TARGET)

# hosted binutils build

$(TOOLCHAIN_HOSTED)/bin/$(TARGET)-ar: binutils-$(BINUTILS_VERSION) $(TOOLCHAIN_SYSROOT)/usr/lib/libc.a
	mkdir -pv build-binutils-$(BINUTILS_VERSION)
	cd build-binutils-$(BINUTILS_VERSION) && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_HOSTED)" --with-sysroot=$(TOOLCHAIN_SYSROOT) --disable-werror
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION)
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION) install

# hosted gcc build

$(TOOLCHAIN_HOSTED)/bin/$(TARGET)-gcc: gcc-$(GCC_VERSION)/gmp $(TOOLCHAIN_HOSTED)/bin/$(TARGET)-ar $(TOOLCHAIN_SYSROOT)/usr/lib/libc.a
	mkdir -pv build-gcc-$(GCC_VERSION)
	cd build-gcc-$(GCC_VERSION) && ../gcc-$(GCC_VERSION)/configure --target=$(TARGET) --prefix="$(TOOLCHAIN_HOSTED)" --with-sysroot=$(TOOLCHAIN_SYSROOT) --enable-languages=c,c++
	$(MAKE) -C build-gcc-$(GCC_VERSION) all-gcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) all-target-libgcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) install-gcc
	$(MAKE) -C build-gcc-$(GCC_VERSION) install-target-libgcc

.PHONY: clean
clean:
	rm -rfv "$(TOOLCHAIN_BARE)"
	rm -rfv "$(TOOLCHAIN_HOSTED)"
	rm -rfv "$(TOOLCHAIN_SYSROOT)"
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION)-bare clean
	$(MAKE) -C build-gcc-$(GCC_VERSION)-bare clean
	$(MAKE) -C build-newlib-$(NEWLIB_VERSION)-bare clean
	$(MAKE) -C build-binutils-$(BINUTILS_VERSION) clean
	$(MAKE) -C build-gcc-$(GCC_VERSION) clean
	$(MAKE) -C build-newlib-$(NEWLIB_VERSION) clean

.PHONY: distclean
distclean:
	rm -rfv "$(TOOLCHAIN_BARE)"
	rm -rfv "$(TOOLCHAIN_HOSTED)"
	rm -rfv "$(TOOLCHAIN_SYSROOT)"
	rm -rfv "build-nasm-$(NASM_VERSION)"
	rm -rfv "build-binutils-$(BINUTILS_VERSION)-bare"
	rm -rfv "build-gcc-$(GCC_VERSION)-bare"
	rm -rfv "build-newlib-$(NEWLIB_VERSION)-bare"
	rm -rfv "build-binutils-$(BINUTILS_VERSION)"
	rm -rfv "build-gcc-$(GCC_VERSION)"
	rm -rfv "build-newlib-$(NEWLIB_VERSION)"
