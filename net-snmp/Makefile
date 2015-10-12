include ../Makefile.in

APP=net-snmp
VERSION=-5.7.3

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ]; then \
		(cd ${APP}${VERSION};\
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
	
build: configure 
	$(call build_package,${APP}${VERSION})

install:
	make -C ${APP}${VERSION} install DESTDIR=${DESTDIR}

clean:
	make -C ${APP}${VERSION} clean

distclean: clean
	$(call distclean_package,${APP}${VERSION})
