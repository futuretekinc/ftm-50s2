include ../Makefile.in

APP=forever
VERSION=-0.11.1

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check

build: configure

install:
	[ -d ${DESTDIR}/usr ] || mkdir -p ${DESTDIR}/usr
	cp -r ${APP}${VERSION}/* ${DESTDIR}/usr/

clean: ;

distclean: clean
	$(call distclean_package,${APP}${VERSION})
