include ../Makefile.in

APP=lua
VERSION=-5.3.1
BUILD_OPTIONS=linux

all: build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check

	
install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured
