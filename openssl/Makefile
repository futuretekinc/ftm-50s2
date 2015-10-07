export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR="${CROSS}ar r"
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=openssl
VERSION=1.0.2d
INSTALLDIR=${CURDIR}/_install
prefix=/usr

all: build install

build:
	make -C ${APP}-${VERSION} CC=${CC} AR=${AR} RANLIB=${RANLIB}

configure:
	cd ${APP}-${VERSION}; ./Configure dist --prefix=${prefix}

install:
	make -C ${APP}-${VERSION} install INSTALL_PREFIX=${INSTALLDIR}

clean:
	make -C ${APP}-${VERSION} clean
