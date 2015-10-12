include ../Makefile.in

APP=pcre
VERSION=-8.37

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}${VERSION}; ./configure --host=${HOST} --prefix=${PREFIX});\
		echo "yes" > .configured; \
	fi;
	
install:
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean

distclean: clean
	$(call distclean_package,${APP}${VERSION})
