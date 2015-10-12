include ../Makefile.in

APP=openssh
VERSION=-7.1p1
SSLDIR=${CURDIR}/../openssl/_install
ZLIBDIR=${CURDIR}/../zlib/_install

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure:
	if [ ! -f .configured ] ; then \
		(cd ${APP}${VERSION}; \
		./configure --host=${HOST} \
			--with-libs \
			--with-zlib=${ZLIBDIR} \
			--with-ssl-dir=${SSLDIR} \
			--disable-etc-default-login \
			--without-hardening \
			--without-stackprotect \
			CC=${CC} \
			LD=${LD} \
			AR=${AR}); \
		echo "yes" > .configured ;\
	fi; 

install:
	make -C ${APP}${VERSION} install-files DESTDIR=${DESTDIR} STRIP_OPT="--strip-program=${STRIP}"

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
