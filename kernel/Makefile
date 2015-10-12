include ../Makefile.in

APP=linux
VERSION=-2.6.37

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	cp config/config.ftm-50s ${APP}${VERSION}/.config
	make -C ${APP}${VERSION} oldconfig

install:
	make -C ${APP}${VERSION} install

clean:
	make -C ${APP}${VERSION} clean
	rm .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
