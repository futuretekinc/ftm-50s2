include ../Makefile.in

APP=zlib
VERSION=-1.2.8

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	cd ${APP}${VERSION}; ./configure --prefix=${PREFIX}

install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -rf .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
