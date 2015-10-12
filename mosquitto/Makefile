include ../Makefile.in


APP=mosquitto
VERSION=-1.4.4
CFLAGS="-I${CURDIR}/../openssl/_install/include"
LDFLAGS="-L${CURDIR}/../openssl/_install/lib -lcrypto -lssl"
BUILD_OPTIONS=	WITH_TLS=no \
		WITH_SRV=no \
		WITH_UUID=no \
		prefix=${PREFIX} \
		CFLAGS=${CFLAGS} \
		LDFLAGS=${LDFLAGS} 
INSTALL_OPTIONS=DESTDIR=${DESTDIR}

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
