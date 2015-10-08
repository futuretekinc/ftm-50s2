export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=webadmin
VERSION=0.1.0
DESTDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION}

configure:

install:
	make -C ${APP}-${VERSION} install DESTDIR=${DESTDIR}
	[ -d ${DESTDIR}/etc ] || mkdir -p ${DESTDIR}/etc
	install -m 644 config/webadmin.conf ${DESTDIR}/etc/webadmin.conf
	[ -d ${DESTDIR}/etc/config ] || mkdir -p ${DESTDIR}/etc/config
	install -m 664 config/config_webadmin ${DESTDIR}/etc/config/webadmin
	[ -d ${DESTDIR}/etc/init.d ] || mkdir -p ${DESTDIR}/etc/init.d
	install -m 755 config/initd_webadmin ${DESTDIR}/etc/init.d/webadmin
	[ -d ${DESTDIR}/etc/rc.d ] || mkdir -p ${DESTDIR}/etc/rc.d
	cd ${DESTDIR}/etc/rc.d; ln -s ../init.d/webadmin S50webadmin	

clean:
	make -C ${APP}-${VERSION} clean
