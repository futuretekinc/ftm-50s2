include ../Makefile.in

BUILD_OPTIONS=CC=${CC} LIB_LOC=/opt/STM/STLinux-2.4/devkit/armv5/target/usr
INSTALL_OPTIONS=DESTDIR=${DESTDIR} PREFIX=${PREFIX}

APP=vsftpd
VERSION=-3.0.3

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		echo "yes" > .configured; \
	fi;

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})
	[ -d ${DESTDIR}/etc ] || mkdir -p ${DESTDIR}/etc
	install -m 644 config/vsftpd.conf ${DESTDIR}/etc
	[ -d ${DESTDIR}/etc/init.d ] || mkdir -p ${DESTDIR}/etc/init.d
	install -m 755 config/initd_vsftpd ${DESTDIR}/etc/init.d/vsftpd

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
