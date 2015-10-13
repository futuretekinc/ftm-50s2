include ../Makefile.in

APP=iptables
VERSION=-1.4.21

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	[ -f .configured ] || cd ${APP}${VERSION};./configure --host=${HOST} --prefix=${PREFIX} --disable-ipv6 CC=${CC}; echo "yes" > .configured

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})
	[ -d ${DESTDIR}/etc/config ] || mkdir -p ${DESTDIR}/etc/config
	install -m 644 config/config_default_firewall ${DESTDIR}/etc/config/firewall
	[ -d ${DESTDIR}/etc/init.d ] || mkdir -p ${DESTDIR}/etc/init.d
	install -m 755 config/initd_firewall ${DESTDIR}/etc/init.d/firewall
	[ -d ${DESTDIR}/etc/rc.d ] || mkdir -p ${DESTDIR}/etc/rc.d
	(cd ${DESTDIR}/etc/rc.d; ln -s ../init.d/firewall S45firewall)

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
