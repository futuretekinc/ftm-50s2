export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip

APP=gmp
VERSION=6.0.0
INSTALLDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION} 

configure:
	cd ${APP}-${VERSION}; \
	./configure --host=arm-linux \
		--prefix=/usr \
		CC=${CC} \
		AR=${AR} 

install:
	make -C ${APP}-${VERSION} install DESTDIR=${INSTALLDIR} 

clean:
	make -C ${APP}-${VERSION} clean
