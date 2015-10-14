TARGET=mmc
COMMON_APPS=busybox openssl iptables zlib openssh libpcap tcpdump lua libubox uci gmp strongswan hotplug2 ntpclient dropbear pcre lighttpd webadmin net-snmp mosquitto node tpgw 

include Makefile.in

all: build
	

configure: 
	for app in $(COMMON_APPS); do \
		make -C $$app configure; \
	done
	
build:  configure
	for app in $(COMMON_APPS); do \
		make -C $$app; \
	done

target: 
	rm -rf ${TARGET}/*;
	tools/make_target ${TARGET};
	tools/make_dev ${TARGET};
	cp -r base/common/* ${TARGET}/
	for app in $(COMMON_APPS); do \
		cp -r $$app/_install/* ${TARGET}/; \
	done
	sudo chown -R 0:0 ${TARGET}/*

clean:
	for app in $(COMMON_APPS); do \
		make -C $$app clean; \
	done

distclean:
	for app in $(COMMON_APPS); do \
		make -C $$app distclean; \
	done

