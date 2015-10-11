export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export PREFIX=/usr

APP=lighttpd
VERSION=1.4.37
export DESTDIR=${CURDIR}/_install
export PCREDIR=../pcre/_install


all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfz ../dl/${APP}-${VERSION}.tar.gz

build:
	make -C ${APP}-${VERSION}

configure:
	if [ ! -f .configured ] ; then \
		(cd ${APP}-${VERSION};\
		./configure --host=${HOST} \
			--prefix=${PREFIX} \
			--enable-shared \
			--without-mysql \
			--without-zlib \
			--without-bzip2 \
			--disable-ipv6 \
			PCRECONFIG=${CURDIR}/${PCREDIR}/usr/bin/pcre-config \
			PCRE_LIB=${CURDIR}/${PCREDIR}/usr/lib/libpcre.a \
			LIBS=-L${CURDIR}/${PCREDIR}/usr/lib \
			CFLAGS="-DHAVE_PCRE_H=1 -DHAVE_LIBPCRE=1 -I${CURDIR}/${PCREDIR}/usr/include"); \
		echo "yes" > .configured; \
	fi;

install:
	make -C ${APP}-${VERSION} install 
	cp -r ${CURDIR}/config/* ${DESTDIR}/

clean:
	make -C ${APP}-${VERSION} clean
