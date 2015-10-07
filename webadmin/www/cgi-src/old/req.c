/******************************************************************************
 * qDecoder - http://www.qdecoder.org
 *
 * Copyright (c) 2000-2012 Seungyoung Kim.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
 * $Id: query.c 636 2012-05-07 23:40:43Z seungyoung.kim $
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "qdecoder.h"
#include "net.h"
#include "pmu_ctrl.h"

#define	FTMC_SEPARATOR	"|!|"

typedef	struct _COMMAND
{
	char	*lpszName;
	int		(*fService)(qentry_t *req);
} FTMC_COMMAND;

int	FTMC_GetDevInfo(qentry_t *pReq);
int	FTMC_GetSessionID(qentry_t *pReq);
int FTMC_GetNetworkStatus(qentry_t *pReq);
int	FTMC_GetDHCPServerStatus(qentry_t *pReq);
int	FTMC_GetDHCPServerConfig(qentry_t *pReq);
int FTMC_GetNATConfig(qentry_t *pReq);

int	FTMC_GetLoginInfo(qentry_t *pReq);
int	FTMC_GetTitle(qentry_t *pReq);
int	FTMC_GetDHCPServerConfig(qentry_t *pReq);
int	FTMC_GetDHCPLeases(qentry_t *pReq);
int	FTMC_GetStaticDHCPLeases(qentry_t *pReq);
int FTMC_SetDHCPServerConfig(qentry_t *pReq);
int	FTMC_GetPM4Status(qentry_t *pReq);
int	FTMC_SetPM4Status(qentry_t *pReq);

int FTMC_GetNetworkConfig(qentry_t *pReq);
int FTMC_SetNetworkConfig(qentry_t *pReq);

FTMC_COMMAND	cmds[] =
{
	{	"get_dev_info",				FTMC_GetDevInfo			},
	{	"get_ssid",					FTMC_GetSessionID		},
	{	"get_network_status",		FTMC_GetNetworkStatus	},
	{	"get_dhcps_status",			FTMC_GetDHCPServerStatus},
	{	"get_dhcps_config",			FTMC_GetDHCPServerConfig},
	{	"get_nat_config",			FTMC_GetNATConfig},

	{	"get_login_info",			FTMC_GetLoginInfo		},
	{	"get_title",				FTMC_GetTitle			},
	{	"get_dhcps_leases",			FTMC_GetDHCPLeases		},
	{	"get_static_dhcp_leases",	FTMC_GetStaticDHCPLeases},
	{	"get_pm4_status",			FTMC_GetPM4Status		},
	{	"set_pm4_status",			FTMC_SetPM4Status		},
	{	"get_network_config",		FTMC_GetNetworkConfig	},
	{	"set_network_config",		FTMC_SetNetworkConfig	},
	{	NULL,						NULL					}
};

FTMC_COMMAND	*FTMC_GetCmd(char *lpszCmd);

int main(void)
{
    // Parse queries.
    qentry_t *pReq = qcgireq_parse(NULL, 0);

    // Get Mode
    char *lpszCmd = (char *)pReq->getstr(pReq, "cmd", false);

    qcgires_setcontenttype(pReq, "text/xml");

	FTMC_COMMAND *pCmd = FTMC_GetCmd(lpszCmd);
	if (pCmd == NULL)
	{
		qcgires_error(pReq, "Invalid query");
	}
	else	
	{
		pCmd->fService(pReq);
	}	

    // De-allocate memories
    pReq->free(pReq);
    return 0;
}

FTMC_COMMAND	*FTMC_GetCmd(char *lpszCmd)
{
	FTMC_COMMAND	*pCmd = &cmds[0];

	if (lpszCmd != NULL)
	{
		while(pCmd->lpszName != NULL)
		{
			if (strcmp(pCmd->lpszName, lpszCmd) == 0)
			{
				return	pCmd;	
			}

			pCmd++;
		}
	}

	return	NULL;
}

int	FTMC_GetSessionID(qentry_t *pReq)
{
	int	ret = 0;
    char	*lpszID = (char *)pReq->getstr(pReq, "id", false);
    char	*lpszPW = (char *)pReq->getstr(pReq, "pw", false);
	
	printf("<SESSION_INFO>\n");

	if ((lpszID != NULL) && (lpszPW != NULL))
	{
		if (strcmp(lpszID, "admin") != 0)
		{
			ret = -1;	
		}

		if (strcmp(lpszPW, "admin") != 0)
		{
			ret = -1;	
		}
	}

	if (ret == 0)
	{
		printf("<SSID>1234567890</SSID>\n");
		printf("<RET>OK</RET>\n");
	}
	else
	{
		printf("<RET>FAILD</RET>\n");
	
	}
	printf("</SESSION_INFO>\n");
}

int	FTMC_GetLoginInfo(qentry_t *pReq)
{
	FILE	*fp = popen("/www/cgi-bin/dev_info.sh", "r");
	if (fp == NULL)
	{
		printf("FTM-Series");	printf(FTMC_SEPARATOR);
		printf("Undefined");	printf(FTMC_SEPARATOR);
		printf("2");			printf(FTMC_SEPARATOR);
		printf("ERR");			printf(FTMC_SEPARATOR);
	}
	else
	{
		char	szBuff[256];		

		if (fgets(szBuff, sizeof(szBuff), fp) != 0)
		{
			printf("%s", szBuff);		printf(FTMC_SEPARATOR);
		}

		if (fgets(szBuff, sizeof(szBuff), fp) != 0)
		{
			printf("%s", szBuff);		printf(FTMC_SEPARATOR);
		}
		printf("2");			printf(FTMC_SEPARATOR);
		printf("ERR");			printf(FTMC_SEPARATOR);
	}

	pclose(fp);
	return	0;

}

int	FTMC_GetNetworkStatus(qentry_t *pReq)
{
	unsigned long	ret;
	FILE	*fp;
	char	szIFName[32];
	char	szMAC[32];
	char	szIP[32];
	char	szSubnetMask[32];
	char	szGateway[32];
	char	szCmdLine[128];
	int		idx;


	printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	printf("<NETWORK_STATUS>\n");

	char	*lpszIfnames[2] = {"eth0", "eth1"};

	for(int i = 0 ; i < 2 ; i++)
	{
		memset(szIFName, 0, sizeof(szIFName));
		memset(szMAC, 0, sizeof(szMAC));
		memset(szIP, 0, sizeof(szIP));
		memset(szSubnetMask, 0, sizeof(szSubnetMask));
		memset(szGateway, 0, sizeof(szGateway));

		sprintf(szCmdLine, "/www/cgi-bin/lan_conf.sh %s", lpszIfnames[i]);

		fp = popen(szCmdLine, "r");
		ret = (unsigned long)fgets(szIFName, sizeof(szIFName), fp);
		ret &= (unsigned long)fgets(szMAC, sizeof(szMAC), fp);
		ret &= (unsigned long)fgets(szIP, sizeof(szIP), fp);
		ret &= (unsigned long)fgets(szSubnetMask, sizeof(szSubnetMask), fp);
		ret &= (unsigned long)fgets(szGateway, sizeof(szSubnetMask), fp);
		pclose(fp);

		if (ret != 0)
		{
			printf("<INTERFACE>\n");
			printf("<IFNAME>%s</IFNAME>\n", lpszIfnames[i]);
			printf("<TYPE>%s</TYPE>\n", "enable");
			printf("<IPADDR>%s</IPADDR>\n", szIP);
			printf("<NETMASK>%s</NETMASK>\n", szSubnetMask);
			printf("<GATEWAY>%s</GATEWAY>\n", szGateway);
			printf("</INTERFACE>\n");
		}
	}

	idx = 0;
	fp = popen("/www/cgi-bin/nameserver.sh", "r");
	while(fgets(szIP, sizeof(szIP), fp) != 0)
	{
		printf("<DNS>");
		printf("<IPADDR>%s</IPADDR>\n", szIP);
		printf("</DNS>\n");
	}
	pclose(fp);

	printf("<RET>OK</RET>\n");
	printf("</NETWORK_STATUS>\n");
	return	0;
}

int	FTMC_GetDHCPServerStatus(qentry_t *pReq)
{
	unsigned long	ret;
	char	szEnable[32];		
	char	szStart[32];		
	char	szEnd[32];		
	char	szLease[32];		

	printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	printf("<DHCP>\n");

	FILE	*fp = popen("/www/cgi-bin/dhcp_conf.sh", "r");

	ret = (unsigned long)fgets(szEnable, sizeof(szEnable), fp);
	ret &= (unsigned long)fgets(szStart, sizeof(szStart), fp);
	ret &= (unsigned long)fgets(szEnd, sizeof(szEnd), fp);
	ret &= (unsigned long)fgets(szLease, sizeof(szLease), fp);
	pclose(fp);

	if (ret == 0)
	{
		printf("<RET>FAILED</RET>\n");	
	}
	else
	{
		printf("<SERVER>\n");
		{
			printf("<DYNAMIC>%s</DYNAMIC>\n", szEnable);	
			printf("<STATIC>%s</STATIC>\n", szEnable);	
			printf("<IP_POOL>\n");
			{
				printf("<START>%s</START>\n", szStart);
				printf("<END>%s</END>\n", szEnd);
				printf("<TIME>%s</TIME>\n", szLease);
			}
			printf("</IP_POOL>\n");
			char	szLine[256];

			FILE *fp = popen("/usr/bin/dumpleases", "r");
			if (fp == NULL)
			{
				printf("<RET>FAILED</RET>\n");	
				return	0;
			}

			fgets(szLine, sizeof(szLine), fp);
			while(fgets(szLine, sizeof(szLine), fp) != NULL)
			{
				char	*lpszMAC;
				char	*lpszIP;
				char	*lpszHostName;
				char	*lpszTime;

				lpszMAC		= strndup(szLine, 18);
				lpszIP		= strndup(szLine + 18, 15);
				lpszHostName= strndup(szLine + 18 + 15, 21);
				lpszTime	= strndup(szLine + 18 + 15 + 21, 16);

				printf("<LEASES>\n");
				printf("<MACADDR>%s</MACADDR>\n", lpszMAC);
				printf("<IPADDR>%s</IPADDR>\n", lpszIP);
				if (lpszHostName[0] != ' ')
				{
					printf("<NAME>%s</NAME>\n", lpszHostName);
				}
				printf("<TIME>%s</TIME>\n", lpszTime);
				printf("</LEASES>\n");

				free(lpszMAC);	
				free(lpszIP);	
				free(lpszHostName);	
				free(lpszTime);	
			}

			pclose(fp);
		}

		printf("</SERVER>\n>");
		printf("<RET>OK</RET>\n");
	}

	printf("</DHCP>\n");
	return	0;
}



int	FTMC_GetDHCPServerConfig(qentry_t *pReq)
{
	unsigned long	ret;
	char	szEnable[32];		
	char	szStart[32];		
	char	szEnd[32];		
	char	szLease[32];		

	printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	printf("<DHCP>\n");

	FILE	*fp = popen("/www/cgi-bin/dhcp_conf.sh", "r");

	ret = (unsigned long)fgets(szEnable, sizeof(szEnable), fp);
	ret &= (unsigned long)fgets(szStart, sizeof(szStart), fp);
	ret &= (unsigned long)fgets(szEnd, sizeof(szEnd), fp);
	ret &= (unsigned long)fgets(szLease, sizeof(szLease), fp);
	pclose(fp);

	if (ret == 0)
	{
		printf("<RET>FAILED</RET>\n");	
	}
	else
	{
		printf("<SERVER>\n");
		{
			printf("<DYNAMIC>%s</DYNAMIC>\n", szEnable);	
			printf("<STATIC>%s</STATIC>\n", szEnable);	
			printf("<IP_POOL>\n");
			{
				printf("<START>%s</START>\n", szStart);
				printf("<END>%s</END>\n", szEnd);
				printf("<TIME>%s</TIME>\n", szLease);
			}
			printf("</IP_POOL>\n");

			FILE	*fp = popen("/www/cgi-bin/static_dhcp_leases.sh", "r");
			char	szEnable[32];
			int		count = 0;

			if (fp == NULL)
			{
				printf("<RET>FAILED</RET>\n");	
				return	0;
			}

			while(1)
			{
				char	szIP[32];
				char	szMAC[32];

				if (fgets(szMAC, sizeof(szMAC), fp) == NULL)
				{
					break;	
				}

				if(fgets(szIP, sizeof(szIP), fp) == NULL)
				{
					break;	
				}

				count++;
				printf("<STATIC_LEASES>\n");
				printf("<MACADDR>%s</MACADDR>\n", szMAC);
				printf("<IPADDR>%s</IPCADDR>\n", szIP);
				printf("</STATIC_LEASES>\n");
			}
		}

		printf("</SERVER>\n>");
		printf("<RET>OK</RET>\n");
	}

	printf("</DHCP>\n");
	return	0;
}


int	FTMC_GetWWANStatus(qentry_t *pReq)
{
	printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	printf("<WWAN>\n");


	printf("</WWAN>\n");
	
	return	0;
}

int	FTMC_GetNATConfig(qentry_t *pReq)
{
	unsigned long	ret;
	char	szEnable[32];		
	char	szStart[32];		
	char	szEnd[32];		
	char	szLease[32];		

	printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
	printf("<NAT>\n");

	return	0;
}

int	FTMC_GetTitle(qentry_t *pReq)
{
	printf("FTM-50/100 3G Router");	printf(FTMC_SEPARATOR);

	return	0;

}

int FTMC_GetStaticDHCPLeases(qentry_t *pReg)
{
	FILE	*fp = popen("/www/cgi-bin/static_dhcp_leases.sh", "r");
	char	szEnable[32];
	int		count = 0;

	if (fp == NULL)
	{
		printf("&nbsp;");	
	}

	printf("dhcp_leases");		printf(FTMC_SEPARATOR);

	if (fgets(szEnable, sizeof(szEnable), fp) != NULL)
	{
		printf("enable");		printf(FTMC_SEPARATOR);	
		while(1)
		{
			char	szIP[32];
			char	szMAC[32];

			if (fgets(szMAC, sizeof(szMAC), fp) == NULL)
			{
				break;	
			}

			if(fgets(szIP, sizeof(szIP), fp) == NULL)
			{
				break;	
			}

			count++;
			printf("%s", szMAC);	printf(FTMC_SEPARATOR);
			printf("%s", szIP);		printf(FTMC_SEPARATOR);
		}

		printf("%d", count);		printf(FTMC_SEPARATOR);
	}
	else
	{
		printf("disabled");		printf(FTMC_SEPARATOR);	
	}
	printf("OK");

	pclose(fp);
	return	0;
}

int	FTMC_GetDHCPLeases(qentry_t *pReq)
{
	char	szLine[256];

	FILE *fp = popen("/usr/bin/dumpleases", "r");
	if (fp == NULL)
	{
		printf("&nbsp;");	
	}

	fgets(szLine, sizeof(szLine), fp);
	while(fgets(szLine, sizeof(szLine), fp) != NULL)
	{
		char	*lpszMAC;
		char	*lpszIP;
		char	*lpszHostName;
		char	*lpszTime;

		lpszMAC		= strndup(szLine, 18);
		lpszIP		= strndup(szLine + 18, 15);
		lpszHostName= strndup(szLine + 18 + 15, 21);
		lpszTime	= strndup(szLine + 18 + 15 + 21, 16);

		printf("%s", lpszMAC);	printf(FTMC_SEPARATOR);
		printf("%s", lpszIP);	printf(FTMC_SEPARATOR);
		if (lpszHostName[0] == ' ')
		{
			printf("&nbsp;");	printf(FTMC_SEPARATOR);
		}
		else
		{
			printf("%s", lpszHostName);	printf(FTMC_SEPARATOR);
		}
		printf("%s", lpszTime);	printf(FTMC_SEPARATOR);

		if (lpszMAC !=NULL)
		{
			free(lpszMAC);	
		}

		if (lpszIP != NULL)
		{
			free(lpszIP);	
		}

		if (lpszHostName != NULL)
		{
			free(lpszHostName);	
		}

		if (lpszTime != NULL)
		{
			free(lpszTime);	
		}
	}

	pclose(fp);

	return	0;
}

int	FTMC_SetDHCPServerConfig(qentry_t *pReq)
{
    char	*lpszDynamic = (char *)pReq->getstr(pReq, "dynamic", false);
    char	*lpszStart = (char *)pReq->getstr(pReq, "ps", false);
    char	*lpszEnd = (char *)pReq->getstr(pReq, "pe", false);
    char	*lpszTime = (char *)pReq->getstr(pReq, "time", false);
    char	*lpszStatic = (char *)pReq->getstr(pReq, "dynamic", false);

    FILE	*fp = fopen("/tmp/udhcpd.conf", "wt");

    fprintf(fp, "# The start and end of the IP lease block\n");
    fprintf(fp, "start	%s\n", lpszStart);
    fprintf(fp, "end    %s\n", lpszEnd);
    fprintf(fp, "\n\n");
    fprintf(fp, "# Options\n");
    fprintf(fp, "opt\tlease\t%s\n", lpszTime);
    fprintf(fp, "\n\n");
    fprintf(fp, "# Static leases map\n");
    if (strcmp(lpszStatic, "enable") == 0)
    {
		char	szName[16];
		char	*lpszLC = (char *)pReq->getstr(pReq, "lc", false);
		int		nCount = 0;
		fprintf(fp, "#static_lease_enabled\n");
		if (lpszLC != NULL)
		{
			nCount = atoi(lpszLC);	
		}

		for(int i = 0 ; i < nCount ; i++)
		{
			sprintf(szName, "mac%d", i);
			char	*lpszMAC = (char *)pReq->getstr(pReq, szName, false);
			sprintf(szName, "ip%d", i);
			char	*lpszIP = (char *)pReq->getstr(pReq, szName, false);

			if (lpszMAC == NULL || lpszIP == NULL)
			{
				fprintf("static_lease\t%s\t%s\n", lpszMAC, lpszIP);	
			}
		}
    }
    else
    {
		fprintf(fp, "#static_lease_disabled\n");
    }

	fclose(fp);

	return	0;
}

int FTMC_GetPM4Status(qentry_t *req)
{
#if 0
	int stat = PMUC_Cmd(4, 0);

	for(int i = 0 ; i < 4 ; i++)
	{
		if ((stat & (1 << i)) != 0)
		{
			printf("ON");	printf(FTMC_SEPARATOR);
		}
		else
		{
			printf("OFF");	printf(FTMC_SEPARATOR);
		}
	}
#else
	FILE	*fp = popen("/www/cgi-bin/pm4 4 0", "r");
	int		stat[4] = {-1, -1, -1, -1};

	for(int i = 0 ; i < 4 ; i++)
	{
		char	buf[16];
		if (fgets(buf, sizeof(buf), fp) != NULL)
		{
			if (buf[0] == '0')
				stat[i] = 0;
			else	
				stat[i] = 1;
		}
	}
	pclose(fp);

	for(int i = 0 ; i < 4 ; i++)
	{
		switch (stat[i])
		{
		case	0: printf("OFF");	printf(FTMC_SEPARATOR);break;
		case	1: printf("ON");	printf(FTMC_SEPARATOR);break;
		default: printf("ERROR");	printf(FTMC_SEPARATOR);break;
		}
	}
#endif
	return	0;
}

int	FTMC_SetPM4Status(qentry_t *req)
{

    char	*lpszID = (char *)req->getstr(req, "id", false);
    char	*lpszValue = (char *)req->getstr(req, "value", false);
	int		nID = -1;
	int		nValue = -1;
	
	nID = atoi(lpszID);
	if (strcmp(lpszValue, "ON") == 0)
	{
		nValue = 0;
	}
	else
	{
		nValue = 1;
	}

	if ((nID < 0 && 3 < nID) || ((nValue != 0) && (nValue != 1)))
	{
		printf("Invalid query\n");
		return	0;
	}

#if 0	
	int nState = PMUC_Cmd(nID, nValue);
	if (nState != -1)
		printf("%s|!|%s|!|%s", lpszID, (nState & (1 << nID))?"ON":"OFF", lpszValue); 
	else
		printf("%s|!|%s|!|%s", lpszID, "ERR", lpszValue); 
#else
	char	cmd[64];

	sprintf(cmd, "/www/cgi-bin/pm4 %d %d\n", nID, nValue);
	FILE	*fp = popen(cmd, "r");
	int		stat[3] = {-1, -1, -1};

	for(int i = 0 ; i < 3 ; i++)
	{
		char	buf[16];
		if (fgets(buf, sizeof(buf), fp) != NULL)
		{
			stat[i] = atoi(buf);
		}
	}
	pclose(fp);

	printf("%s|!|%s|!|%s", lpszID, (stat[2] == 1)?"ON":((stat[2] == 0)?"OFF":"ERR"), lpszValue); 
#endif

	return	0;
}


int	FTMC_GetNetworkConfig(qentry_t *pReq)
{
	unsigned long	ret;
	FILE	*fp;
	char	szIFName[32];
	char	szMAC[32];
	char	szIP[32];
	char	szSubnetMask[32];
	char	szGateway[32];
	char	szCmdLine[128];
	int		idx;


	char	*lpszIfnames[2] = {"eth0", "eth1"};

	for(int i = 0 ; i < 2 ; i++)
	{
		memset(szIFName, 0, sizeof(szIFName));
		memset(szMAC, 0, sizeof(szMAC));
		memset(szIP, 0, sizeof(szIP));
		memset(szSubnetMask, 0, sizeof(szSubnetMask));
		memset(szGateway, 0, sizeof(szGateway));

		sprintf(szCmdLine, "/www/cgi-bin/lan_conf.sh %s", lpszIfnames[i]);

		fp = popen(szCmdLine, "r");
		ret = (unsigned long)fgets(szIFName, sizeof(szIFName), fp);
		ret &= (unsigned long)fgets(szMAC, sizeof(szMAC), fp);
		ret &= (unsigned long)fgets(szIP, sizeof(szIP), fp);
		ret &= (unsigned long)fgets(szSubnetMask, sizeof(szSubnetMask), fp);
		ret &= (unsigned long)fgets(szGateway, sizeof(szSubnetMask), fp);
		pclose(fp);

		if (ret != 0)
		{
			printf("%s", lpszIfnames[i]);printf(FTMC_SEPARATOR);
			printf("enable");			printf(FTMC_SEPARATOR);
			printf("%s", szIP);			printf(FTMC_SEPARATOR);
			printf("%s", szSubnetMask);	printf(FTMC_SEPARATOR);
			printf("%s", szGateway);	printf(FTMC_SEPARATOR);
		}
	}

	idx = 0;
	fp = popen("/www/cgi-bin/nameserver.sh", "r");
	while(fgets(szIP, sizeof(szIP), fp) != 0)
	{
		printf("dns%d", idx++);		printf(FTMC_SEPARATOR);
		printf("%s", szIP);			printf(FTMC_SEPARATOR);	
	}
	pclose(fp);

	printf("OK");
	return	0;
}

int FTMC_SetNetworkConfig(qentry_t *pReq)
{
	FILE	*fp = fopen("/tmp/interfaces", "w");

	fprintf(fp,"auto lo\n");
	fprintf(fp,"iface lo inet loopback\n");
	fprintf(fp,"\n");

    char	*lpszType= (char *)pReq->getstr(pReq, "type0", false);
    char	*lpszIP= (char *)pReq->getstr(pReq, "ip0", false);
    char	*lpszSubnetMask= (char *)pReq->getstr(pReq, "netmask0", false);
    char	*lpszGateway = (char *)pReq->getstr(pReq, "gateway0", false);

	printf("eth0");					printf(FTMC_SEPARATOR);
	printf("%s", lpszType);			printf(FTMC_SEPARATOR);
	if (strcmp(lpszType, "enable") == 0)
	{
		fprintf(fp,"auto eth0\n");	
		fprintf(fp,"face eth0 inet static\n");	
		fprintf(fp,"\taddress %s\n", lpszIP);
		fprintf(fp,"\tnetmask %s\n", lpszSubnetMask);
		fprintf(fp,"\tgateway %s\n", lpszGateway);
		fprintf(fp,"\tpre-up [-f /etc/network/local-network-ok ]\n");
		fprintf(fp,"\n");
		
		printf("%s", lpszIP);			printf(FTMC_SEPARATOR);
		printf("%s", lpszSubnetMask);	printf(FTMC_SEPARATOR);
		printf("%s", lpszGateway);		printf(FTMC_SEPARATOR);
	}
	else if (strcmp(lpszType, "DHCP") == 0)
	{
		fprintf(fp,"auto eth0\n");	
		fprintf(fp,"face eth0 inet dhcp\n");	
		fprintf(fp,"\tpre-up [-f /etc/network/local-network-ok ]\n");
		fprintf(fp,"\n");
	}

    lpszType= (char *)pReq->getstr(pReq, "type1", false);
    lpszIP= (char *)pReq->getstr(pReq, "ip1", false);
    lpszSubnetMask= (char *)pReq->getstr(pReq, "netmask1", false);
    lpszGateway = (char *)pReq->getstr(pReq, "gateway1", false);

	printf("eth1");					printf(FTMC_SEPARATOR);
	printf("%s", lpszType);			printf(FTMC_SEPARATOR);
	if (strcmp(lpszType, "enable") == 0)
	{
		fprintf(fp,"auto eth1\n");	
		fprintf(fp,"face eth1 inet static\n");	
		fprintf(fp,"\taddress %s\n", lpszIP);
		fprintf(fp,"\tnetmask %s\n", lpszSubnetMask);
		fprintf(fp,"\tgateway %s\n", lpszGateway);
		fprintf(fp,"\tpre-up [-f /etc/network/local-network-ok ]\n");
		fprintf(fp,"\n");

		printf("%s", lpszIP);			printf(FTMC_SEPARATOR);
		printf("%s", lpszSubnetMask);	printf(FTMC_SEPARATOR);
		printf("%s", lpszGateway);		printf(FTMC_SEPARATOR);
	}
	else if (strcmp(lpszType, "DHCP") == 0)
	{
		fprintf(fp,"auto eth1\n");	
		fprintf(fp,"face eth1 inet dhcp\n");	
		fprintf(fp,"\tpre-up [-f /etc/network/local-network-ok ]\n");
		fprintf(fp,"\n");
	}
	fclose(fp);

	char	*nameserver0 = (char *)pReq->getstr(pReq, "dns0", false);
	char	*nameserver1 = (char *)pReq->getstr(pReq, "dns1", false);
	fp = fopen("/tmp/resolv.conf", "w");
	if(nameserver0 != NULL)
	{
		fprintf(fp, "nameserver %s\n", nameserver0);
		printf("dns0");				printf(FTMC_SEPARATOR);
		printf("%s", nameserver0);	printf(FTMC_SEPARATOR);
	}

	if(nameserver1 != NULL)
	{
		fprintf(fp, "nameserver %s\n", nameserver1);
		printf("dns1");				printf(FTMC_SEPARATOR);
		printf("%s", nameserver1);	printf(FTMC_SEPARATOR);
	}

	fclose(fp);

	printf("OK");

	return	0;
}

