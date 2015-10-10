export HOST=arm-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install

APP=dropbear
VERSION=2015.68

CONFIGDIR=/etc/config
INITDDIR=/etc/init.d
RCDDIR=/etc/rc.d

all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfz ${APP}-${VERSION}.tar.gz

build:
	make -C ${APP}-${VERSION} \
		STATIC=1 \
		SCPPROGRESS=0 \
		PROGRAMS="dropbear dropbearkey scp dbclient"

configure:
	if [ ! -f .configured ]; then \
		(cd ${APP}-${VERSION};\
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

install:
	make -C ${APP}-${VERSION} install DESTDIR=${DESTDIR}
	[ -d ${DESTDIR}${CONFIGDIR} ] || mkdir -p ${DESTDIR}${CONFIGDIR}
	install -m 644 config/config_dropbear ${DESTDIR}${CONFIGDIR}/${APP}
	[ -d ${DESTDIR}${INITDDIR} ] || mkdir -p ${DESTDIR}${INITDDIR}
	install -m 755 config/initd_dropbear ${DESTDIR}${INITDDIR}/${APP}
	[ -d ${DESTDIR}${RCDDIR} ] || mkdir -p ${DESTDIR}${RCDDIR}
	cd ${DESTDIR}${RCDDIR}; ln -sf ${INITDDIR}/${APP} S50${APP};ln -sf ${INITDDIR}/${APP} K50${APP};


clean:
	make -C ${APP}-${VERSION} clean
