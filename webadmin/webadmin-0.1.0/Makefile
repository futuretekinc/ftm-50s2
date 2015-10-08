prefix=/www

all: build

build: 
	make -C cgi

configure:

install:
	[ -d ${DESTDIR}${prefix} ] || mkdir -p ${DESTDIR}${prefix}
	cp -r www/* ${DESTDIR}${prefix}
	make -C cgi install 

clean:
	make -C cgi clean
