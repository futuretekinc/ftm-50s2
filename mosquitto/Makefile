export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export CXX=${CROSS}g++
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install
export CFLAGS="-I${CURDIR}/../openssl/_install/include"
export LDFLAGS="-L${CURDIR}/../openssl/_install/lib -lcrypto -lssl"

APP=mosquitto
VERSION=1.4.4

all: check build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfz ${APP}-${VERSION}.tar.gz

build:
	make -C ${APP}-${VERSION} \
		WITH_TLS=no \
		WITH_SRV=no \
		WITH_UUID=no \
		prefix=${PREFIX} \
		CFLAGS=${CFLAGS} \
		LDFLAGS=${LDFLAGS} 

configure:

install:
	make -C ${APP}-${VERSION} install DESTDIR=${DESTDIR} 

clean:
	make -C ${APP}-${VERSION} clean
