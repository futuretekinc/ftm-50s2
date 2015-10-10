export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export DESTDIR=${CURDIR}/_install

APP=hotplug2
VERSION=1.0

all: check build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfz ${APP}-${VERSION}.tar.gz

build:
	make -C ${APP}-${VERSION} 

configure:

install:
	make -C ${APP}-${VERSION} install DESTDIR=${DESTDIR}
	cp -r config/* ${DESTDIR}/

clean:
	make -C ${APP}-${VERSION} clean
