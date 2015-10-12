include ../Makefile.in

APP=webadmin
VERSION=-0.1.0

all: confiugre build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check

install:
	make -C ${APP}${VERSION} install DESTDIR=${DESTDIR}
	[ -d ${DESTDIR}/etc ] || mkdir -p ${DESTDIR}/etc
	install -m 644 config/webadmin.conf ${DESTDIR}/etc/webadmin.conf
	[ -d ${DESTDIR}/etc/config ] || mkdir -p ${DESTDIR}/etc/config
	install -m 664 config/config_webadmin ${DESTDIR}/etc/config/webadmin
	[ -d ${DESTDIR}/etc/init.d ] || mkdir -p ${DESTDIR}/etc/init.d
	install -m 755 config/initd_webadmin ${DESTDIR}/etc/init.d/webadmin
	[ -d ${DESTDIR}/etc/rc.d ] || mkdir -p ${DESTDIR}/etc/rc.d
	cd ${DESTDIR}/etc/rc.d; ln -s ../init.d/webadmin S50webadmin	

clean:
	make -C ${APP}${VERSION} clean
	rm -rf .configured
