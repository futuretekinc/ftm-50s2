include ../Makefile.in

APP=libpcap
VERSION=-1.7.4

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}${VERSION}; \
			./configure --host=${HOST} \
				--with-pcap=linux \
				ac_cv_linux_vers=2 \
				--prefix=${PREFIX} \
				CC=${CC} \
				AR=${AR});\
		echo "yes" > .configured; \
	fi;

install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured
