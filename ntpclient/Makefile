include ../Makefile.in

APP=ntpclient
VERSION=_2010_365
BINDIR=${DESTDIR}/usr/sbin

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	

install:
	[ -d ${DESTDIR}/usr/sbin ] || mkdir -p ${BINDIR}

	install -m 755 ${APP}${VERSION}/${APP} ${BINDIR}/${APP}
	cp -r config/* ${DESTDIR}/

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
