export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install

APP=gmp
VERSION=6.0.0

all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfj ${APP}-${VERSION}.tar.bz2

build:
	make -C ${APP}-${VERSION} 

configure:
	if [ ! -f .configured ] ; then \
		(cd ${APP}-${VERSION}; ./configure --host=${HOST} --prefix=${PREFIX} CC=${CC} AR=${AR});\
		echo "yes" > .configured;\
	fi;

install:
	make -C ${APP}-${VERSION} install 

clean:
	make -C ${APP}-${VERSION} clean
