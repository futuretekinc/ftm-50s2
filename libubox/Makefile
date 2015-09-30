export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=libubox
VERSION=
INSTALLDIR=${PWD}/_install

all: build install

build:
	make -C ${APP} 

configure:
	cd ${APP}; cmake -D CMAKE_C_COMPILER=${CC} -D CMAKE_INSTALL_PREFIX:PATH=/usr .

install:
	make -C ${APP} install DESTDIR=${INSTALLDIR}

clean:
	make -C ${APP} clean
