include ../Makefile.in

APP=tcpdump
VERSION=-4.7.4

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ]; then \
		(cd ${APP}${VERSION}; \
		./configure --host=${HOST} \
			--disable-ipv6 \
			--with-crypto=no\
			ac_cv_linux_vers=2 \
			--prefix=${PREFIX} \
			CC=${CC} \
			AR=${AR} );\
		echo "yes" > .configured;\
	fi;

install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
