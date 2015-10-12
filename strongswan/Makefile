include ../Makefile.in

APP=strongswan
VERSION=-5.3.3

GMPDIR=${CURDIR}/../gmp/_install/usr

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

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

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
