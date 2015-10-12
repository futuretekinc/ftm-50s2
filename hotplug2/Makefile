include ../Makefile.in

APP=hotplug2
VERSION=-1.0

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check

install:
	make -C ${APP}${VERSION} install DESTDIR=${DESTDIR}
	cp -r config/* ${DESTDIR}/

clean:
	make -C ${APP}${VERSION} clean
	rm -rf .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
