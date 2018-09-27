DASH_VERSION=0.5.10

.PHONY: all
all: dash

.PHONY: dash
dash: $(SYSROOT)/bin/dash

# downloads

dash-$(DASH_VERSION).tar.gz:
	wget http://gondor.apana.org.au/~herbert/dash/files/$@

# patches

dash-$(DASH_VERSION): dash-$(DASH_VERSION).tar.gz
	tar -xvf $<
	patch -p0 < shk-dash-$(DASH_VERSION).patch

# dash build

build-dash-$(DASH_VERSION): dash-$(DASH_VERSION)
	mkdir -pv $@
	export PATH="$(TOOLCHAIN_PREFIX)/bin:$$PATH"; cd $@ && ../$</configure --host=$(TARGET) --prefix="/" --enable-static

$(SYSROOT)/bin/dash: build-dash-$(DASH_VERSION)
	export PATH="$(TOOLCHAIN_PREFIX)/bin:$$PATH"; $(MAKE) -C $< CC=$(TARGET)-gcc
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
