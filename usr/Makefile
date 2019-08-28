DASH_VERSION=0.5.10
COREUTILS_VERSION=8.31
BUSYBOX_VERSION=1.30.1

.PHONY: all
all: dash

.PHONY: dash
dash: $(SYSROOT)/bin/dash

#.PHONY: coreutils
#coreutils: $(SYSROOT)/bin/ls

.PHONY: busybox
busybox: $(SYSROOT)/bin/ls

# downloads

dash-$(DASH_VERSION).tar.gz:
	wget http://gondor.apana.org.au/~herbert/dash/files/$@

coreutils-$(COREUTILS_VERSION).tar.xz:
	wget https://ftp.gnu.org/gnu/coreutils/$@

busybox-$(BUSYBOX_VERSION).tar.bz2:
	wget https://busybox.net/downloads/$@

# patches

dash-$(DASH_VERSION): dash-$(DASH_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-dash-$(DASH_VERSION).patch

coreutils-$(COREUTILS_VERSION): coreutils-$(COREUTILS_VERSION).tar.xz
	tar -xvf $<

busybox-$(BUSYBOX_VERSION): busybox-$(BUSYBOX_VERSION).tar.bz2
	tar -xvf $<

# configure

build-dash-$(DASH_VERSION): dash-$(DASH_VERSION)
	mkdir -pv $@
	export PATH="$(TOOLCHAIN_PREFIX)/bin:$$PATH"; cd $@ && ../$</configure --host=$(TARGET) --prefix="/" --enable-static --enable-debug

build-coreutils-$(COREUTILS_VERSION): coreutils-$(COREUTILS_VERSION)
	mkdir -pv $@

build-busybox-$(BUSYBOX_VERSION): busybox-$(BUSYBOX_VERSION)
	mkdir -pv $@

# build and install

$(SYSROOT)/bin/dash: build-dash-$(DASH_VERSION)
	export PATH="$(TOOLCHAIN_PREFIX)/bin:$$PATH"; $(MAKE) -C $< CC=$(TARGET)-gcc CFLAGS='-O0 -g'
	export PATH="$(TOOLCHAIN_PREFIX)/bin:$$PATH"; $(MAKE) -C $< install-exec CC=$(TARGET)-gcc DESTDIR="$(SYSROOT)"

# configure --host=i386-shk --enable-static --prefix=??? --sysroot???
# make CC=i386-shk-gcc
# make install-exec # to avoid installing man page
#

.PHONY: clean
clean:
	rm -fv "$(SYSROOT)/bin/dash"
	$(MAKE) -C build-dash-$(DASH_VERSION) clean

.PHONY: distclean
distclean:
	rm -fv "$(SYSROOT)/bin/dash"
	rm -rfv "build-dash-$(DASH_VERSION)"
