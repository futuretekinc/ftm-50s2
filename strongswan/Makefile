export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR="${CROSS}ar"
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export GMPDIR=${CURDIR}/../gmp/_install/usr

APP=strongswan
VERSION=5.2.2
INSTALLDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION} DESTDIR=${INSTALLDIR}

configure:
	cd ${APP}-${VERSION}; ./configure \
		--host=arm-linux \
		--prefix=/usr \
		--sysconfdir=/etc \
		--with-lib-prefix=${GMPDIR} \
		CC=${CC} AR=${AR} RANLIB=${RANLIB}

install:
	make -C ${APP}-${VERSION} install DESTDIR=${INSTALLDIR}

clean:
	make -C ${APP}-${VERSION} clean
