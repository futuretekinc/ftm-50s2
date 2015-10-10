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

APP=pcre
VERSION=8.37
export DESTDIR=${CURDIR}/_install

all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfz ../dl/${APP}-${VERSION}.tar.gz

build:
	make -C ${APP}-${VERSION}

configure:
	[ -f .configured ] || cd ${APP}-${VERSION};./configure --host=${HOST} -prefix=${PREFIX}; echo "yes" > .configured

install:
	make -C ${APP}-${VERSION} install 

clean:
	rm -f .configured
	make -C ${APP}-${VERSION} clean
