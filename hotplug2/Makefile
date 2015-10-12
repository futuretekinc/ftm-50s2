include ../Makefile.in

APP=hotplug2
VERSION=-1.0

INSTALL_OPTIONS=DESTDIR=${DESTDIR}

all: configure build install

check:
	$(call check_package,${APP}${VERSION})


configure: check

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})
	cp -r config/* ${DESTDIR}/

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
