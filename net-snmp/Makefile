export HOST=armv5-linux
export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib
export PREFIX=/usr
export DESTDIR=${CURDIR}/_install

APP=net-snmp
VERSION=5.7.3

all: check configure build install

check:
	[ -d ${APP}-${VERSION} ] || tar xvfz ${APP}-${VERSION}.tar.gz

build:
	make -C ${APP}-${VERSION} 

configure:
	if [ ! -f .configured ]; then \
		(cd ${APP}-${VERSION};\
		./configure \
			--host=${HOST} \
			--target=${HOST} \
			--with-defaults \
			--with-endianness=little \
			--disable-embedded-perl \
			--disable-manuals \
			--without-perl-modules \
			--with-default-snmp-version="2" \
			--with-sys-contact="@@no.where" \
			--with-sys-location="Unknown" \
			--with-logfile="/var/log/snmpd.log" \
			--with-persistent-directory="/var/net-snmp" \
			--prefix=${PREFIX} \
			--libdir=${PREFIX}/lib); \
		echo "yes" > .configured;\
	fi;
	
install:
	make -C ${APP}-${VERSION} install DESTDIR=${DESTDIR}

clean:
	make -C ${APP}-${VERSION} clean
