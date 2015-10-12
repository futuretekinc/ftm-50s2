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
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean

distclean: clean
	$(call distclean_package,${APP}${VERSION})
