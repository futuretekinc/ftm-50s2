export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export DESTDIR=${CURDIR}/_install

APP=lua
VERSION=5.3.1

all: check build install

check:
	[ -d ${APP}-${VERSION} ] || (tar xvfz ${APP}-${VERSION}.tar.gz; patch -p0 < patch/${APP}-${VERSION}.diff)

build:
	make -C ${APP}-${VERSION} linux

configure:
	
install:
	make -C ${APP}-${VERSION} install 

clean:
	make -C ${APP}-${VERSION} clean
