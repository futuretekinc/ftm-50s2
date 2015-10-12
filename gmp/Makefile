include ../Makefile.in

APP=gmp
VERSION=-6.0.0

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}${VERSION}; ./configure --host=${HOST} --prefix=${PREFIX} CC=${CC} AR=${AR});\
		echo "yes" > .configured;\
	fi;

install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -rf .configured
