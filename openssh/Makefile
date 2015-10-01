export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip

APP=openssh
VERSION=7.1p1
SSLDIR=${PWD}/../openssl/_install
ZLIBDIR=${PWD}/../zlib/_install
INSTALLDIR=${PWD}/_install

all: build install

build:
	make -C ${APP}-${VERSION}  CC=${CC} LD=${LD} AR=${AR}

configure:
	cd ${APP}-${VERSION}; \
	./configure --host=arm-linux \
		--with-libs \
		--with-zlib=${ZLIBDIR} \
		--with-ssl-dir=${SSLDIR} \
		--disable-etc-default-login \
		--without-hardening \
		--without-stackprotect \
		CC=${CC} \
		LD=${LD} \
		AR=${AR} 

install:
	make -C ${APP}-${VERSION} install-files DESTDIR=${INSTALLDIR} STRIP_OPT="--strip-program=armv5-linux-strip"

clean:
	make -C ${APP}-${VERSION} clean
