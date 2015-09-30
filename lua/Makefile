export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR="${CROSS}ar r"
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=lua
VERSION=5.3.1
INSTALLDIR=${PWD}/_install

all: build install

build:
	make -C ${APP}-${VERSION} linux

configure:

install:
	make -C ${APP}-${VERSION} install

clean:
	make -C ${APP}-${VERSION} clean
