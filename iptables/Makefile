export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip

APP=iptables
VERSION=1.4.21
INSTALLDIR=${PWD}/_install

all: build install

build:
	make -C ${APP}-${VERSION} CC=${CC}

configure:
	cd ${APP}-${VERSION}; ./configure --host=arm-linux --prefix=/usr --disable-ipv6 CC=${CC}

install:
	make -C ${APP}-${VERSION} install

clean:
	make -C ${APP}-${VERSION} clean
