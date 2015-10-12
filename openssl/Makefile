include ../Makefile.in

APP=openssl
VERSION=-1.0.2d
BUILD_OPTIONS=AR="${AR} r"

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}${VERSION}; ./Configure dist --prefix=${PREFIX}); \
		echo "yes" > .configured; \
	fi

install:
	make -C ${APP}${VERSION} install INSTALL_PREFIX=${DESTDIR}

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
