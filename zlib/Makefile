export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export TARGETMACH=arm-linux

APP=zlib
VERSION=1.2.8
INSTALLDIR=${PWD}/_install

all: build install

build:
	make -C ${APP}-${VERSION} 

configure:
	cd ${APP}-${VERSION}; ./configure --prefix=${INSTALLDIR}

install:
	make -C ${APP}-${VERSION} install 

clean:
	make -C ${APP}-${VERSION} clean
