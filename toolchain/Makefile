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
	wget http://www.nasm.us/pub/nasm/releasebuilds/$(NASM_VERSION)/$@

binutils-$(BINUTILS_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/binutils/$@

gcc-$(GCC_VERSION).tar.gz:
	wget ftp://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/$@

newlib-$(NEWLIB_VERSION).tar.gz:
	wget ftp://sourceware.org/pub/newlib/$@

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

newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/kernel/syscall.h: $(SHK_INCLUDE_DIR)/kernel/syscall.h newlib-$(NEWLIB_VERSION)
	mkdir -pv $(dir $@)
	cp -v $< $@

newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/syscalls.c: $(SHK_LIBC_SOURCE_DIR)/syscalls.c newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/kernel/syscall.h newlib-$(NEWLIB_VERSION)
	cp -v $< $@

newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/crt0.c: $(SHK_LIBC_SOURCE_DIR)/crt0.c newlib-$(NEWLIB_VERSION)
	cp -v $< $@

# gcc prerequisites

gcc-$(GCC_VERSION)/gmp: gcc-$(GCC_VERSION)
	cd $< && sh contrib/download_prerequisites
	touch $@

# nasm build

build-nasm-$(NASM_VERSION): nasm-$(NASM_VERSION)
	mkdir -pv $@
	cd $@ && ../$</configure --prefix="$(TOOLCHAIN_HOSTED)"

$(TOOLCHAIN_HOSTED)/bin/nasm: build-nasm-$(NASM_VERSION)
	$(MAKE) -C $<
	$(MAKE) -C $< install

# bare binutils build

build-binutils-$(BINUTILS_VERSION)-bare: binutils-$(BINUTILS_VERSION)
	mkdir -pv $@
	cd $@ && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_BARE)" --with-sysroot --disable-nls --disable-werror

$(TOOLCHAIN_BARE)/bin/$(TARGET)-ar: build-binutils-$(BINUTILS_VERSION)-bare
	$(MAKE) -C $<
	$(MAKE) -C $< install

# bare gcc build

build-gcc-$(GCC_VERSION)-bare: gcc-$(GCC_VERSION) gcc-$(GCC_VERSION)/gmp $(TOOLCHAIN_BARE)/bin/$(TARGET)-ar
	mkdir -pv $@
	cd $@ && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_BARE)" --disable-nls --enable-languages=c,c++ --without-headers

$(TOOLCHAIN_BARE)/bin/$(TARGET)-gcc: build-gcc-$(GCC_VERSION)-bare
	$(MAKE) -C $< all-gcc
	$(MAKE) -C $< all-target-libgcc
	$(MAKE) -C $< install-gcc
	$(MAKE) -C $< install-target-libgcc

# bare newlib build

build-newlib-$(NEWLIB_VERSION)-bare: newlib-$(NEWLIB_VERSION) $(TOOLCHAIN_BARE)/bin/$(TARGET)-ar $(TOOLCHAIN_BARE)/bin/$(TARGET)-gcc
	mkdir -pv $@
	export PATH="$(TOOLCHAIN_BARE)/bin:$$PATH"; cd $@ && ../$</configure --target=$(TARGET) --prefix=/usr

$(TOOLCHAIN_SYSROOT)/usr/lib/libc.a: build-newlib-$(NEWLIB_VERSION)-bare newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/syscalls.c newlib-$(NEWLIB_VERSION)/newlib/libc/sys/shk/crt0.c
	export PATH="$(TOOLCHAIN_BARE)/bin:$$PATH"; $(MAKE) -C $< all
	export PATH="$(TOOLCHAIN_BARE)/bin:$$PATH"; $(MAKE) -C $< DESTDIR=$(TOOLCHAIN_SYSROOT) install
	rm -rfv "$(TOOLCHAIN_SYSROOT)/usr/include" "$(TOOLCHAIN_SYSROOT)/usr/lib"
	mv -v "$(TOOLCHAIN_SYSROOT)/usr/$(TARGET)/include" "$(TOOLCHAIN_SYSROOT)/usr/$(TARGET)/lib" "$(TOOLCHAIN_SYSROOT)/usr"
	rmdir -v $(TOOLCHAIN_SYSROOT)/usr/$(TARGET)

# hosted binutils build

build-binutils-$(BINUTILS_VERSION): binutils-$(BINUTILS_VERSION)
	mkdir -pv $@
	cd $@ && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_HOSTED)" --with-sysroot=$(TOOLCHAIN_SYSROOT) --disable-werror

$(TOOLCHAIN_HOSTED)/bin/$(TARGET)-ar: build-binutils-$(BINUTILS_VERSION) $(TOOLCHAIN_SYSROOT)/usr/lib/libc.a
	$(MAKE) -C $<
	$(MAKE) -C $< install

# hosted gcc build

build-gcc-$(GCC_VERSION): gcc-$(GCC_VERSION) gcc-$(GCC_VERSION)/gmp $(TOOLCHAIN_HOSTED)/bin/$(TARGET)-ar
	mkdir -pv $@
	cd $@ && ../$</configure --target=$(TARGET) --prefix="$(TOOLCHAIN_HOSTED)" --with-sysroot=$(TOOLCHAIN_SYSROOT) --enable-languages=c,c++

$(TOOLCHAIN_HOSTED)/bin/$(TARGET)-gcc: build-gcc-$(GCC_VERSION) $(TOOLCHAIN_SYSROOT)/usr/lib/libc.a
	$(MAKE) -C $< all-gcc
	$(MAKE) -C $< all-target-libgcc
	$(MAKE) -C $< install-gcc
	$(MAKE) -C $< install-target-libgcc

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
