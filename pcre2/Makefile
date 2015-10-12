include ../Makefile.in

APP=pcre2
VERSION=-10.20

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	cd ${APP}${VERSION};./configure --host=${HOST} -prefix=${PREFIX}

install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured
