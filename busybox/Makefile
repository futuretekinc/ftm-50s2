export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export DESTDIR=${CURDIR}/_install

APP=busybox
VERSION=1.23.2

all: check build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfj ${APP}-${VERSION}.tra.bz2

build:
	make -C ${APP}-${VERSION} CC=${CC}

configure:
	cp config/config.ftm-50s2 ${APP}-${VERSION}/.config
	make -C ${APP}-${VERSION} oldconfig

install:
	make -C ${APP}-${VERSION} install

clean:
	make -C ${APP}-${VERSION} clean
