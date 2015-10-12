include ../Makefile.in

APP=libubox
VERSION=

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}; cmake -D CMAKE_C_COMPILER=${CC} -D CMAKE_INSTALL_PREFIX:PATH=${PREFIX} .);\
		echo "yes" > .configured; \
	fi;

install:
	make -C ${APP} install 

clean:
	make -C ${APP} clean
	rm -f .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
