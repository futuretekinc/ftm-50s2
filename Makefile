TARGET=mmc
COMMON_APPS=busybox openssl iptables zlib openssh libpcap tcpdump

all: build

build: 
	for app in $(COMMON_APPS); do \
		make -C $$app; \
	done


target_gen: 
	rm -rf ${TARGET}/*
	cp -r base/common/* ${TARGET}/
	tools/make_dev
	for app in $(COMMON_APPS); do \
		cp -r $$app/_install/* ${TARGET}/; \
	done

target_tpgw:
	rm -rf ${TARGET}/*
	cp -r base/common/* ${TARGET}/;
	cp -r base/tpgw/* ${TARGET}/;
	tools/make_dev;
	for app in $(cOMMON_APPS); do \
		cp -r $$app/_install/* ${TARGET}/; \
	done

clean:
	for app in $(APPS); do \
		make -C $$app clean; \
	done

