include ../Makefile.in

APP=libpcap
VERSION=-1.7.4

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

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

build: configure 
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
