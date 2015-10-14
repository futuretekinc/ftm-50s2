include ../Makefile.in

BUILD_OPTIONS=AR="${AR} r"
INSTALL_OPTIONS=INSTALL_PREFIX=${DESTDIR}

APP=openssl
VERSION=-1.0.2d

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(export CLFAGS=-fPIC; cd ${APP}${VERSION}; ./Configure \
		--prefix=${PREFIX} shared linux-armv5); \
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
