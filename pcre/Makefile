include ../Makefile.in

APP=pcre
VERSION=-8.37

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	[ -f .configured ] || cd ${APP}${VERSION};./configure --host=${HOST} -prefix=${PREFIX}; echo "yes" > .configured

install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
