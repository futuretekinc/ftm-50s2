include ../Makefile.in

APP=zlib
VERSION=-1.2.8

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	cd ${APP}${VERSION}; ./configure --prefix=${PREFIX}

build: configure 
	$(call build_package,${APP}${VERSION})

install: build
	$(call install_package,${APP}${VERSION})

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
