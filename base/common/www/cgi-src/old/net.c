#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "net.h"

int get_default_gateway(const char* ifname, char *gateway) ;

int get_net_info(char *ifname, struct net_info *info)
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;

	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);

	//ioctl(fd, SIOCGIFHWADDR, &ifr);
	//sprintf(info->hwaddr, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_hwaddr)->sin_addr));

	ioctl(fd, SIOCGIFADDR, &ifr);
	info->ip = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;

	ioctl(fd, SIOCGIFNETMASK, &ifr);
	info->netmask = ((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr;

	close(fd);

	FILE* fp = popen("netstat -rn", "r");
	char line[256]={0x0};
	int	ret = -1;

	while(fgets(line, sizeof(line), fp) != NULL)
	{    
		/*
		 * Get destination.
		 */
		char* destination;
		destination = strndup(line, 15);

		/*
		 * Extract iface to compare with the requested one
		 * todo: fix for iface names longer than eth0, eth1 etc
		 */
		char* iface;
		iface = strndup(line + 73, 4);


		// Find line with the gateway
		if(strcmp("0.0.0.0        ", destination) == 0 && strcmp(iface, ifname) == 0) 
		{	
			char ip[16];
			// Extract gateway
			strncpy(ip, line + 16, 15);
			ip[15] = 0;
			char *spos = strchr(ip, ' ');
			if (spos != NULL)
			{
				*spos = NULL;	
			}

			inet_aton(ip, &(info->gateway));
		}

		free(destination);
		free(iface);
	}

	pclose(fp);

	fp = fopen("/etc/resolv.conf", "r");

	info->nDNS = 0;
	while((info->nDNS < 2) && (fgets(line, sizeof(line), fp) != NULL))
	{
		char	name[256], ip[256];	

		sscanf(line, "%s %s", name, ip);
		if (strcmp(name, "nameserver") == 0)
		{
			if (inet_aton(ip, &info->dns[info->nDNS]) != 0)
			{
				info->nDNS ++;	
			}
		}
	}

	fclose(fp);
	return 0;
}



int FTMC_LoadNetworkInterfaceFile(NET_IFS *pIFS)
{
	FILE	*fp = fopen("/etc/network/interfaces", "r");
	char	szLine[256];
	int		index = 0;

	if (fp == NULL)
	{
		return	-1;	
	}

	while(fgets(szLine, sizeof(szLine), fp) != 0)
	{
		if (strncmp("auto", szLine, 4) == 0)
		{
			int	stat = 0;

			while(fgets(szLine, sizeof(szLine), fp) != 0)
			{
				switch(stat)
				{
				case	0:
					{
							
					}
					break;
				}
			}
		}
	}
		
		


}









