include ../Makefile.in

APP=libubox
VERSION=

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}; cmake -D CMAKE_C_COMPILER=${CC} -D CMAKE_INSTALL_PREFIX:PATH=${PREFIX} .);\
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
