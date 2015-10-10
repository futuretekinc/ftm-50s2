export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export PREFIX=/usr

APP=iptables
VERSION=1.4.21
export DESTDIR=${CURDIR}/_install

all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfj ../dl/${APP}-${VERSION}.tar.bz2

build:
	make -C ${APP}-${VERSION} CC=${CC}

configure:
	[ -f .configured ] || cd ${APP}-${VERSION};./configure --host=${HOST} --prefix=${PREFIX} --disable-ipv6 CC=${CC}; echo "yes" > .configured

install:
	make -C ${APP}-${VERSION} install 

clean:
	rm -f .configured
	make -C ${APP}-${VERSION} clean
