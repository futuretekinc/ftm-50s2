include ../Makefile.in

BUILD_OPTIONS=BUILD_CC=gcc CC=${CC}
APP=libcap
VERSION=-2.24

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}${VERSION}; \
			./configure --host=${HOST} \
				--prefix=${PREFIX} \
				CC=${CC} \
				AR=${AR});\
		echo "yes" > .configured; \
	fi;

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
