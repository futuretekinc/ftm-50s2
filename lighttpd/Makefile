export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=lighttpd
VERSION=1.4.37
INSTALLDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION}

configure:
	make -C ${app}-${VERSION} --host=arm-linux -prefix=/usr

install:
	make -C ${APP}-${VERSION} install DESTDIR=${INSTALLDIR}

clean:
	make -C ${APP}-${VERSION} clean
