export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=net-snmp
VERSION=5.7.3
INSTALLDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION} 
configure:
	cd ${APP}-${VERSION};\
	./configure \
		--host=armv5-linux \
		--target=armv5-linux \
		--with-endianness=little \
		--disable-embedded-perl \
		--without-perl-modules \
		--prefix=${CURDIR}/_install/usr \
		--libdir=${CURDIR}/_install/usr/lib
	
install:
	make -C ${APP}-${VERSION} install DESTDIR=${INSTALLDIR}

clean:
	make -C ${APP}-${VERSION} clean
