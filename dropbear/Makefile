export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip

APP=dropbear
VERSION=2015.68
INSTALLDIR=${PWD}/_install

all: build install

build:
	make -C ${APP}-${VERSION} \
		MULTI=1 \
		SCPPROGRESS=0 \
		PROGRAMS="dropbear dropbearkey scp dbclient ssh"

configure:
	cd ${APP}-${VERSION};\
	./configure --host=arm-linux \
		--disable-zlib \
		--disable-largefile \
		--disable-loginfunc \
		--disable-shadow \
		--disable-utmp \
		--disable-utmpx \
		--disable-wtmp \
		--disable-wtmpx \
		--disable-pututline \
		--disable-pututxline \
		--disable-lastlog \
		--prefix=/usr \
		CC=${CC} 

install:
	make -C ${APP}-${VERSION} install DESTDIR=${INSTALLDIR}

clean:
	make -C ${APP}-${VERSION} clean
