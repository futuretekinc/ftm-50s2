export CROSS=armv5-linux-
export CC=${CROSS}gcc
export AR=${CROSS}ar
export LD=${CROSS}ld
export AS=${CROSS}as
export STRIP=${CROSS}strip
export RANLIB=${CORSS}ranlib

APP=perl
VERSION=5.22.0
INSTALLDIR=${CURDIR}/_install

all: build install

build:
	make -C ${APP}-${VERSION} 
configure:
	cd ${APP}-${VERSION};\
	./Configure -des \
		-Dusecrosscomplie \
		-Dtargethost=192.168.1.10 \
		-Dtargetuser=root \
		-Dtargetarch=arm-linux \
		-Dcc=armv5-linux-gcc \
		-Dusrinc=/opt/STM/STLinux-2.4/devkit/armv5/include \
		-Dincpth=/opt/STM/STLinux-2.4/devkit/armv5/include \
		-Dlibpth=/opt/STM/STLinux-2.4/devkit/armv5/lib \
		-Dtargetdir=${CURDIR}/_install/usr	
install:
	make -C ${APP}-${VERSION} install DESTDIR=${INSTALLDIR}

clean:
	make -C ${APP}-${VERSION} clean
