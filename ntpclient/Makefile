export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=ntpclient
VERSION=2010
INSTALLDIR=${CURDIR}/_install
BINDIR=${INSTALLDIR}/usr/sbin
CONFIGDIR=${INSTALLDIR}/etc/config

all: build install

build:
	make -C ${APP}-${VERSION}

configure:
	

install:
	[ -d ${INSTALLDIR}/usr/sbin ] || mkdir -p ${BINDIR}

	install -m 755 ${APP}-${VERSION}/${APP} ${BINDIR}/${APP}

	[ -d ${INSTALLDIR}/etc/config ] || mkdir -p ${CONFIGDIR}

	install -m 644 config/${APP} ${CONFIGDIR}/${APP}
clean:
	make -C ${APP}-${VERSION} clean
