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

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	make -C ${APP}${VERSION} install DESTDIR=${DESTDIR} 

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured

distclean: clean
	$(call distclean_package,${APP}${VERSION})
