#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"


int	FTMC_Netfilter(qentry_t *pReq)
{
    char	*lpszCmd = pReq->getstr(pReq, "cmd", false);

    if (strcmp(lpszCmd, "status") == 0)
    {
		char	szBuf[1024];
		FILE *pPF = popen("iptables-save | awk -f /www/cgi-bin/status_iptables.awk", "r");
		if (pPF != NULL)
		{
			printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
			printf("<STATUS_NETWORK>\n");
			while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
			{
				char	szType[64];

				sscanf(szBuf, "%s", szType);
				if (strcmp(szType, "TABLE") == 0)
				{
					char	szName[64];

					sscanf(szBuf, "%s %s", szType, szName);
					printf("<TABLE>%s</TABLE>\n", szName);
				}
				else if (strcmp(szType, "CHAIN") == 0)
				{
					char	szName[64], szPolicy[64];

					sscanf(szBuf, "%s %s %s", szType, szName, szPolicy);
					printf("<%s><%s>%s</%s></%s>\n", szType, szName, szPolicy, szName, szType);
				}
				else if (strcmp(szType, "RULE") == 0)
				{
					char	szPipe[64], szDir[64], szInterface[64]; 
					char	szSIP[64], szDIP[64], szProto[64]; 
					char	szSPort[64], szDPort[64], szPolicy[64];

					sscanf(szBuf, "%s %s %s %s %s %s %s %s %s %s", 
						szName, szPipe, szDir, szInterface, 
						szSIP, szDIP, szProto, 
						szSPort, szDPort, szPolicy);

					printf("<%s>\n", szType);
					printf("<PIPE>%s</PIPE>\n", szPipe);
					printf("<DIR>%s</DIR>\n", szDir);
					printf("<IF>%s</IF>\n", szInterface);
					printf("<SIP>%s</SIP>\n", szSIP);
					printf("<DIP>%s</DIP>\n", szDIP);
					printf("<PROTO>%s</PROTO>\n", szProt);
					printf("<SPORT>%s</SPORT>\n", szSPort);
					printf("<DPORT>%s</DPORT>\n", szDPort);
					printf("<POLICY>%s</POLICY>\n", szPolicy);
					printf("</%s>\n", szType);
				}
			}
			pclose(pPF);
		}

    }

    return	0;
}

