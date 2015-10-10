export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export TARGETMACH=arm-linux
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install

APP=zlib
VERSION=1.2.8

all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfz ${APP}-${VERSION}.tar.gz

build:
	make -C ${APP}-${VERSION} 

configure:
	cd ${APP}-${VERSION}; ./configure --prefix=${PREFIX}

install:
	make -C ${APP}-${VERSION} install 

clean:
	make -C ${APP}-${VERSION} clean
