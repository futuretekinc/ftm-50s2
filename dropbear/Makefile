include ../Makefile.in

APP=dropbear
VERSION=-2015.68

CONFIGDIR=/etc/config
INITDDIR=/etc/init.d
RCDDIR=/etc/rc.d
BUILD_OPTIONS=STATIC=1 SCPPROGRESS=0 PROGRAMS="dropbear dropbearkey scp dbclient"

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ]; then \
		(cd ${APP}${VERSION};\
		./configure --host=${HOST} \
			--disable-zlib \
			--disable-largefile \
			--disable-loginfunc \
			--disable-shadow \
			--disable-utmp \
			--disable-utmpx \
			--disable-wtmp \
			--disable-wtmpx \
			--disable-pututline \
			--disable-pututxline \
			--disable-lastlog \
			--prefix=${PREFIX} \
			CC=${CC});\
		echo "yes" > .configured;\
	fi;

build: configure
	$(call build_package,${APP}${VERSION})

install:
	make -C ${APP}${VERSION} install DESTDIR=${DESTDIR}
	[ -d ${DESTDIR}${CONFIGDIR} ] || mkdir -p ${DESTDIR}${CONFIGDIR}
	install -m 644 config/config_dropbear ${DESTDIR}${CONFIGDIR}/${APP}
	[ -d ${DESTDIR}${INITDDIR} ] || mkdir -p ${DESTDIR}${INITDDIR}
	install -m 755 config/initd_dropbear ${DESTDIR}${INITDDIR}/${APP}
	[ -d ${DESTDIR}${RCDDIR} ] || mkdir -p ${DESTDIR}${RCDDIR}
	cd ${DESTDIR}${RCDDIR}; ln -sf ${INITDDIR}/${APP} S50${APP};ln -sf ${INITDDIR}/${APP} K50${APP};


clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
