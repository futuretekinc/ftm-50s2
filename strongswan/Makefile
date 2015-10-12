include ../Makefile.in

APP=strongswan
VERSION=-5.3.3

GMPDIR=${CURDIR}/../gmp/_install/usr

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ]; then\
		(cd ${APP}${VERSION}; ./configure \
			--host=${HOST} \
			--prefix=${PREFIX} \
			--sysconfdir=/etc \
			--with-lib-prefix=${GMPDIR} \
			CC=${CC} AR=${AR} RANLIB=${RANLIB});\
		echo "yes" > .configured;\
	fi;

install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured
