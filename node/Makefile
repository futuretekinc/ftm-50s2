include ../Makefile.in

APP=node
VERSION=-0.10.28

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
