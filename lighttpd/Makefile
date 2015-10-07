export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=lighttpd
VERSION=1.4.37
INSTALLDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION}

configure:
	cd ${APP}-${VERSION};\
	./configure --host=arm-linux --prefix=/usr --enable-shared --without-mysql --without-zlib --without-bzip2 --disable-ipv6 \
		PCRECONFIG=${CURDIR}/../pcre/_install/usr/bin/pcre-config \
		PCRE_LIB=${CURDIR}/../pcre/_install/usr/lib/libpcre.a \
		LIBS=-L${CURDIR}/../pcre/_install/usr/lib \
		CFLAGS="-DHAVE_PCRE_H=1 -DHAVE_LIBPCRE=1 -I${CURDIR}/../pcre/_install/usr/include"

install:
	make -C ${APP}-${VERSION} install DESTDIR=${INSTALLDIR}
	cp -r ${CURDIR}/config/* ${INSTALLDIR}/

clean:
	make -C ${APP}-${VERSION} clean
