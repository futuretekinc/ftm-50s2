include ../Makefile.in

APP=linux
VERSION=-2.6.37

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	cp config/config.ftm-50s ${APP}${VERSION}/.config
	make -C ${APP}${VERSION} oldconfig

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
