export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=hotplug2
VERSION=1.0
INSTALLDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION} 

configure:

install:
	make -C ${APP}-${VERSION} install DESTDIR=${INSTALLDIR}
	cp -r config/* ${INSTALLDIR}/

clean:
	make -C ${APP}-${VERSION} clean
