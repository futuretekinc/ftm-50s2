export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export DESTDIR=${CURDIR}/_install

APP=openssh
VERSION=7.1p1
SSLDIR=${CURDIR}/../openssl/_install
ZLIBDIR=${CURDIR}/../zlib/_install

all: configure build install

build:
	make -C ${APP}-${VERSION}  CC=${CC} LD=${LD} AR=${AR}

configure:
	if [ ! -f .configured ] ; then \
		(cd ${APP}-${VERSION}; \
		./configure --host=${HOST} \
			--with-libs \
			--with-zlib=${ZLIBDIR} \
			--with-ssl-dir=${SSLDIR} \
			--disable-etc-default-login \
			--without-hardening \
			--without-stackprotect \
			CC=${CC} \
			LD=${LD} \
			AR=${AR}); \
		echo "yes" > .configured ;\
	fi; 

install:
	make -C ${APP}-${VERSION} install-files DESTDIR=${DESTDIR} STRIP_OPT="--strip-program=${STRIP}"

clean:
	make -C ${APP}-${VERSION} clean
