include ../Makefile.in

USER=`whoami`
GROUP=`whoami`
INSTALL_OPTIONS=DESTDIR=${DESTDIR} INSTALL_STRIP="--strip-program=${STRIP}" INSTALL_USER=${USER} INSTALL_GROUP=${GROUP}
APP=proftpd
VERSION=-1.3.5a

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
