include ../Makefile.in

APP=lighttpd
VERSION=-1.4.37
PCREDIR=../pcre/_install

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	if [ ! -f .configured ] ; then \
		(cd ${APP}${VERSION};\
		./configure --host=${HOST} \
			--prefix=${PREFIX} \
			--enable-shared \
			--without-mysql \
			--without-zlib \
			--without-bzip2 \
			--disable-ipv6 \
			PCRECONFIG=${CURDIR}/${PCREDIR}/usr/bin/pcre-config \
			PCRE_LIB=${CURDIR}/${PCREDIR}/usr/lib/libpcre.a \
			LIBS=-L${CURDIR}/${PCREDIR}/usr/lib \
			CFLAGS="-DHAVE_PCRE_H=1 -DHAVE_LIBPCRE=1 -I${CURDIR}/${PCREDIR}/usr/include"); \
		echo "yes" > .configured; \
	fi;

install:
	make -C ${APP}${VERSION} install 
	cp -r ${CURDIR}/config/* ${DESTDIR}/

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured
