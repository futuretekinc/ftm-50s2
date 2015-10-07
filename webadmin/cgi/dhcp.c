#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"

int FTMC_DHCP(qentry_t *pReq)
{

	char *lpszCmd = pReq->getstr(pReq, "cmd", false);

	if (strcmp(lpszCmd, "status") == 0)
	{
		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<DHCP_SERVER>\n");

		char	szBuf[1024];
		FILE *pPF = popen("/www/cgi-bin/status_dhcp.sh", "r");
		if (pPF != NULL)
		{
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
				char	szTag[64];	
				char	szData0[64];
				char	szData1[64];
				
				switch(sscanf(szBuf, "%s %s %s", szTag, szData0, szData1))
				{
				case	2:
					{
						if (strcmp(szTag, "STATUS:") == 0)
						{
							printf("<STATUS>%s</STATUS>\n", szData0);					
						}
						else if (strcmp(szTag, "INTERFACE:") == 0)
						{
							printf("<INTERFACE>%s</INTERFACE>\n", szData0);					
						}
						else if (strcmp(szTag, "START:") == 0)
						{
							printf("<START>%s</START>\n", szData0);					
						}
						else if (strcmp(szTag, "END:") == 0)
						{
							printf("<END>%s</END>\n", szData0);					
						}
						else if (strcmp(szTag, "ROUTER:") == 0)
						{
							printf("<ROUTER>%s</ROUTER>\n", szData0);					
						}
						else if (strcmp(szTag, "TIME:") == 0)
						{
							printf("<TIME>%s</TIME>\n", szData0);					
						}
						else if (strcmp(szTag, "STATIC:") == 0)
						{
							printf("<STATIC>%s</STATIC>\n", szData0);					
						}
						else if (strcmp(szTag, "DNS1:") == 0)
						{
							printf("<DNS1>%s</DNS1>\n", szData0);					
						}
						else if (strcmp(szTag, "DNS2:") == 0)
						{
							printf("<DNS2>%s</DNS2>\n", szData0);					
						}

					}
					break;

				case	3:
					{
						if (strcmp(szTag, "STATIC_LEASE:") == 0)
						{
							printf("<STATIC_LEASE>\n");
							printf("<MAC>%s</MAC>\n", szData0);
							printf("<IP>%s</IP>\n", szData1);
							printf("</STATIC_LEASE>\n");
						}
					}		
				}
			}
			pclose(pPF);
		}


		pPF = popen("/usr/bin/dumpleases | awk -f /www/cgi-bin/parse_dumpleases.awk", "r");
		if (pPF != NULL)
		{
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
				char	szTag[64];	
				char	szData[3][64];

				int		nArgs = sscanf(szBuf, "%s %s %s %s", szTag, szData[0], szData[1], szData[2]);

				if (strcmp(szTag, "<START>") == 0)
				{
					printf("<LEASE>\n");
				}
				else if (strcmp(szTag, "<END>") == 0)
				{
					printf("</LEASE>\n");
				}
				else if (strcmp(szTag, "MAC:") == 0)
				{
					printf("<MAC>%s</MAC>\n", szData[0]);
				}
				else if (strcmp(szTag, "IP:") == 0)
				{
					printf("<IP>%s</IP>\n", szData[0]);
				}
				else if (strcmp(szTag, "HOSTNAME:") == 0)
				{
					printf("<HOSTNAME>");
					if (nArgs > 1)
					{
						for(int i = 0 ; i < nArgs - 1; i++)
						{
							printf("%s ", szData[i]);
						}
					}
					printf("</HOSTNAME>\n");
				}
				else if (strcmp(szTag, "EXPIRESIN:") == 0)
				{
					printf("<EXPIRESIN>");
					if (nArgs > 1)
					{
						for(int i = 0 ; i < nArgs - 1; i++)
						{
							printf("%s ", szData[i]);
						}
					}
					printf("</EXPIRESIN>\n");
				}
				
			}

			pclose(pPF);
		}

		printf("</DHCP_SERVER>\n");
	}
	else if (strcmp(lpszCmd, "config") == 0)
	{
	}
	else if(strcmp(lpszCmd, "set") == 0)
	{
		char *lpszEnable	= pReq->getstr(pReq, "enable", false);
		char *lpszInterface = pReq->getstr(pReq, "if", false);
		char *lpszStatic	= pReq->getstr(pReq, "static", false);
		char *lpszStart		= pReq->getstr(pReq, "start", false);
		char *lpszEnd		= pReq->getstr(pReq, "end", false);
		char *lpszRouter	= pReq->getstr(pReq, "router", false);
		char *lpszTime		= pReq->getstr(pReq, "time", false);
		char *lpszDNS1		= pReq->getstr(pReq, "dns1", false);
		char *lpszDNS2		= pReq->getstr(pReq, "dns2", false);

		if ((lpszEnable == NULL) || 
			(lpszInterface == NULL) || 
			(lpszStatic == NULL) || 
			(lpszStart == NULL) || 
			(lpszEnd == NULL) || 
			(lpszTime == NULL) ||
			(lpszDNS1 == NULL) ||
			(lpszDNS2 == NULL))
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<DHCP_CONFIG>\n");	
			printf("<RET>ERROR</RET>\n");
			printf("<MSG>Invalid Parameter!</MSG>\n");
			printf("</DHCP_CONFIG>\n");	
		}

		FILE *fp = fopen("/tmp/udhcpd.conf", "w");
		if (fp == NULL)
		{
			qcgires_setcontenttype(pReq, "text/xml");
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<DHCP_CONFIG>\n");	
			printf("<RET>SYSTEM_ERROR</RET>\n");
			printf("<MSG>Can't create file\n[/tmp/udhcpd.conf]</MSG>\n");
			printf("</DHCP_CONFIG>\n");	
			return	-1;	
		}

		fprintf(fp, "static %d\n", ((lpszStatic != NULL) && (strcmp(lpszStatic, "true") == 0)));
		fprintf(fp, "\n");

		fprintf(fp, "# The start and end of the IP lease block\n");
		if ((lpszStart != NULL) && (lpszEnd != NULL))
		{
			fprintf(fp, "start %s\n", lpszStart);
			fprintf(fp, "end   %s\n", lpszEnd);
		}
		fprintf(fp, "\n");

		fprintf(fp, "# The interface that udhcpd will use\n");
		fprintf(fp, "interface %s\n", lpszInterface);	
		fprintf(fp, "\n");

		fprintf(fp, "# Options\n");
		if (lpszTime != NULL)
		{
			fprintf(fp, "opt lease %s\n", lpszTime);
		}
		fprintf(fp, "\n");

		if (lpszRouter != NULL)
		{
			fprintf(fp, "opt router %s\n", lpszRouter);
		}
		fprintf(fp, "\n");

		fprintf(fp, "opt subnet 255.255.255.0\n");
		fprintf(fp, "\n");

		if (lpszDNS1 != NULL)
		{
			fprintf(fp, "opt dns %s\n", lpszDNS1);
		}
		fprintf(fp, "\n");
		
		if (lpszDNS2 != NULL)
		{
			fprintf(fp, "opt dns %s\n", lpszDNS2);
		}
		fprintf(fp, "\n");

		fprintf(fp, "# Static leases map\n");
		for(int i = 0 ; ; i++)
		{
			char *lpszMAC = (char *)pReq->getstrf(pReq, false, "mac%d", i);
			char *lpszIP = (char *)pReq->getstrf(pReq, false, "ip%d", i);

			if ((lpszMAC == NULL) || (lpszIP == NULL))
			{
				break;
			}

			for(int i = 0 ; i < strlen(lpszMAC) ; i++)
			{
				lpszMAC[i] = toupper(lpszMAC[i]);
			}

			if ((lpszStatic != NULL) && (strcmp(lpszStatic, "true") == 0))
			{
				fprintf(fp, "static_lease %s %s\n", lpszMAC, lpszIP);
			}
			else
			{
				fprintf(fp, "# static_lease %s %s\n", lpszMAC, lpszIP);
			}
		}
		fclose(fp);

		fp = popen("cp -f /tmp/udhcpd.conf /etc/udhcpd.conf;sync;sync", "r");
		pclose(fp);

		if (strcmp(lpszEnable, "true") == 0)
		{
			fp = popen("cp /etc/init.d/save/udhcpd /etc/init.d/;sync;sync;service udhcpd start;", "r");
			pclose(fp);
			//usleep(500);
			fp = popen("service udhcpd restart", "r");
			pclose(fp);
		}
		else
		{
			fp = popen("service udhcpd stop; rm /etc/init.d/udhcpd;sync;sync", "r");
			pclose(fp);
		}

		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<DHCP_CONFIG>\n");
		printf("<RET>OK</RET>\n");
		printf("</DHCP_CONFIG>\n");
	}
	else
	{
		return	-1;	
	}

	return	0;
}
