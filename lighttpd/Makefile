include ../Makefile.in

APP=lighttpd
VERSION=-1.4.37
PCREDIR=../pcre
PCREDESTDIR=${PCREDIR}/_install

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		if [ ! -d ${PCREDESTDIR} ]; then \
			make -C ${PCREDIR}; \
		fi; \
		(cd ${APP}${VERSION};\
		./configure --host=${HOST} \
			--prefix=${PREFIX} \
			--enable-shared \
			--without-mysql \
			--without-zlib \
			--without-bzip2 \
			--disable-ipv6 \
			PCRECONFIG=${CURDIR}/${PCREDESTDIR}/usr/bin/pcre-config \
			PCRE_LIB=${CURDIR}/${PCREDESTDIR}/usr/lib/libpcre.a \
			LIBS=-L${CURDIR}/${PCREDESTDIR}/usr/lib \
			CFLAGS="-DHAVE_PCRE_H=1 -DHAVE_LIBPCRE=1 -I${CURDIR}/${PCREDESTDIR}/usr/include"); \
		echo "yes" > .configured; \
	fi;

build: configure
	$(call build_package,${APP}${VERSION})

install:
	$(call install_package,${APP}${VERSION})
	cp -r ${CURDIR}/config/* ${DESTDIR}/

clean:
	$(call clean_package,${APP}${VERSION})

distclean: clean
	$(call distclean_package,${APP}${VERSION})
