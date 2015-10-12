include ../Makefile.in

APP=ntpclient
VERSION=-2010
BINDIR=${DESTDIR}/usr/sbin

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	
build: configure 
	$(call build_package,${APP}${VERSION})

install:
	[ -d ${DESTDIR}/usr/sbin ] || mkdir -p ${BINDIR}

	install -m 755 ${APP}${VERSION}/${APP} ${BINDIR}/${APP}
	cp -r config/* ${DESTDIR}/

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
