include ../Makefile.in

APP=node
VERSION=-0.10.28

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build: ;

configure: check

install:
	[ -d ${DESTDIR}/usr ] || mkdir -p ${DESTDIR}/usr
	cp -r ${APP}${VERSION}/* ${DESTDIR}/usr/

clean:
