export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install

APP=tcpdump
VERSION=4.7.4

all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfz ${APP}-${VERSION}.tar.gz

build:
	make -C ${APP}-${VERSION} 

configure:
	if [ ! -f .configured ]; then \
		(cd ${APP}-${VERSION}; \
		./configure --host=${HOST} \
			--disable-ipv6 \
			--with-crypto=no\
			ac_cv_linux_vers=2 \
			--prefix=${PREFIX} \
			CC=${CC} \
			AR=${AR} );\
		echo "yes" > .configured;\
	fi;

install:
	make -C ${APP}-${VERSION} install 

clean:
	make -C ${APP}-${VERSION} clean
