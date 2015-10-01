export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}"ar r"
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=openssl
VERSION=1.0.2d
INSTALLDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION} CC=${CC} AR=${AR} RANLIB=${RANLIB}

configure:
	cd ${APP}-${VERSION}; ./Configure dist --prefix=${INSTALLDIR}

install:
	make -C ${APP}-${VERSION} install

clean:
	make -C ${APP}-${VERSION} clean
