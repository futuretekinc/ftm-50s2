include ../Makefile.in

APP=perl
VERSION=-5.22.0

all: configure build install

check:
	$(call check_package,${APP}${VERSION})

build:
	$(call build_package,${APP}${VERSION})

configure: check
	cd ${APP}${VERSION};\
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
	make -C ${APP}${VERSION} install 

clean:
	make -C ${APP}${VERSION} clean
	rm -f .configured

distclean:
	$(call distclean_package,${APP}${VERSION})
