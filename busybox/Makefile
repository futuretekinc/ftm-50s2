include ../Makefile.in

APP=busybox
VERSION=-1.23.2

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	cp config/config.ftm-50s2 ${APP}${VERSION}/.config
	make -C ${APP}${VERSION} oldconfig

build: configure
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})
	chmod +s ${DESTDIR}/bin/busybox
	[ -d ${DESTDIR}/etc ] || mkdir -p ${DESTDIR}/etc
	install -m 644 config/busybox.conf ${DESTDIR}/etc

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
