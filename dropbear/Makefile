export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip

APP=dropbear
VERSION=2015.68
DESTDIR=${CURDIR}/_install
CONFIGDIR=/etc/config
INITDDIR=/etc/init.d
RCDDIR=/etc/rc.d

all: build install

build:
	make -C ${APP}-${VERSION} \
		STATIC=1 \
		SCPPROGRESS=0 \
		PROGRAMS="dropbear dropbearkey scp dbclient"

configure:
	cd ${APP}-${VERSION};\
	./configure --host=arm-linux \
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
		--prefix=/usr \
		CC=${CC} 

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
