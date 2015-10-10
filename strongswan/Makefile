export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR="${CROSS}ar"
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install

APP=strongswan
VERSION=5.3.3

GMPDIR=${CURDIR}/../gmp/_install/usr

all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || (tar xvfz ../dl/${APP}-${VERSION}.tar.gz; patch -p0 < patch/${APP}-${VERSION}.diff)

build:
	make -C ${APP}-${VERSION} 

configure:
	if [ ! -f .configured ]; then\
		(cd ${APP}-${VERSION}; ./configure \
			--host=${HOST} \
			--prefix=${PREFIX} \
			--sysconfdir=/etc \
			--with-lib-prefix=${GMPDIR} \
			CC=${CC} AR=${AR} RANLIB=${RANLIB});\
		echo "yes" > .configured;\
	fi;

install:
	make -C ${APP}-${VERSION} install 

clean:
	make -C ${APP}-${VERSION} clean
