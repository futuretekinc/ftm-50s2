export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR="${CROSS}ar r"
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install

APP=openssl
VERSION=1.0.2d

all: configure build install

build:
	make -C ${APP}-${VERSION} CC=${CC} AR=${AR} RANLIB=${RANLIB}

configure:
	if [ ! -f .configured ] ; then \
		(cd ${APP}-${VERSION}; ./Configure dist --prefix=${PREFIX}); \
		echo "yes" > .configured; \
	fi

install:
	make -C ${APP}-${VERSION} install INSTALL_PREFIX=${DESTDIR}

clean:
	make -C ${APP}-${VERSION} clean
