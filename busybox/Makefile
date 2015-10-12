include ../Makefile.in

APP=busybox
VERSION=-1.23.2

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	cp config/config.ftm-50s2 ${APP}${VERSION}/.config
	make -C ${APP}${VERSION} oldconfig

install:
	make -C ${APP}${VERSION} install

clean:
	make -C ${APP}${VERSION} clean
	rm .configured
