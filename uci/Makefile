export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install

APP=uci
VERSION=

all: check configure build install

check:
	[ -d ${APP} ] || tar xvfz ../dl/${APP}.tar.gz

build:
	make -C ${APP} 

configure:
	if [ ! -f .configured ]; then \
		(cd ${APP}; cmake -D CMAKE_C_COMPILER=${CC} -D CMAKE_INSTALL_PREFIX:PATH=${PREFIX} -D CMAKE_LIBUBOX_DIR:=${CURDIR}/../libubox/_install/usr .); \
		echo "yes" > .configured; \
	fi;

install:
	make -C ${APP} install 

clean:
	make -C ${APP} clean
