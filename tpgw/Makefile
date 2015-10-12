include ../Makefile.in

APP=tpgw
VERSION=-2015.06.22

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check

build: configure 

install:
	[ -d ${DESTDIR}/opt ] || mkdir -p ${DESTDIR}/opt
	cp -r ${APP}${VERSION}/* ${DESTDIR}/opt/
	[ -d ${DESTDIR}/etc/init.d ] || mkdir -p ${DESTDIR}/etc/init.d
	install -m 755 config/initd_tpgw ${DESTDIR}/etc/init.d/tpgw
	[ -d ${DESTDIR}/etc/rc.d ] || mkdir -p ${DESTDIR}/etc/rc.d
	(cd ${DESTDIR}/etc/rc.d; ln -sf ../init.d/tpgw S97tpgw)

clean: ;

distclean: clean
	$(call distclean_package,${APP}${VERSION})
