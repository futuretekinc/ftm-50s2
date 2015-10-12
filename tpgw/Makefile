include ../Makefile.in

APP=tpgw
VERSION=-2015.06.15

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build: ;

configure: check

install:
	[ -d ${DESTDIR}/opt ] || mkdir -p ${DESTDIR}/opt
	cp -r src/* ${DESTDIR}/opt/
	[ -d ${DESTDIR}/usr ] || mkdir -p ${DESTDIR}/usr
	cp -r node_usr/local ${DESTDIR}/usr/
	[ -d ${DESTDIR}/etc/init.d ] || mkdir -p ${DESTDIR}/etc/init.d
	install -m 755 config/initd_tpgw ${DESTDIR}/etc/init.d/tpgw
	[ -d ${DESTDIR}/etc/rc.d ] || mkdir -p ${DESTDIR}/etc/rc.d
	(cd ${DESTDIR}/etc/rc.d; ln -sf ../init.d/tpgw S97tpgw)

clean:
