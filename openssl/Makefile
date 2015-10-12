include ../Makefile.in

APP=openssl
VERSION=-1.0.2d
BUILD_OPTIONS=AR="${AR} r"
INSTALL_OPTIONS=INSTALL_PREFIX=${DESTDIR}

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}${VERSION}; ./Configure dist --prefix=${PREFIX}); \
		echo "yes" > .configured; \
	fi

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
