#include <stdio.h>

#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int get_net_info(struct net_info *info)
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;

	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

	ioctl(fd, SIOCGIFHWADDR, &ifr);
	info->hwaddr= ifr.ifr_addr;

	ioctl(fd, SIOCGIFADDR, &ifr);
	info->ipaddr = ifr.ifr_addr;


	ioctl(fd, SIOCGIFNETMASK, &ifr);
	info->netmask= ifr.ifr_addr;

	close(fd);
	return 0;
}
