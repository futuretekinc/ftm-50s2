#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"
#include "debug.h"
#include "util.h"

int	FTMC_Netfilter(qentry_t *pReq)
{
    char	*lpszCmd = pReq->getstr(pReq, "cmd", false);

    if (strcmp(lpszCmd, "status") == 0)
    {
		char	szBuf[1024];
		FILE *fp;
	
		XML_PutHeader(pReq);

		printf("<STATUS_NETWORK>\n");

		fp = fopen("/etc/service/iptables", "r");
		if (fp != NULL)
		{
			if ((fscanf(fp, "%s", szBuf) == 1) && (strcmp(szBuf, "enabled") == 0))
			{
				printf("<STATE>enabled</STATE>\n");
			}
			else
			{
				printf("<STATE>disabled</STATE>\n");

			}
			fclose(fp);
		}
		else
		{
			printf("<STATE>disabled</STATE>\n");
		}

		fp = popen("cat /etc/iptables | awk -f /www/cgi-bin/status_iptables.awk", "r");
		if (fp != NULL)
		{

			while(fgets(szBuf, sizeof(szBuf), fp) != 0)
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
						szType, szPipe, szDir, szInterface, 
						szSIP, szDIP, szProto, 
						szSPort, szDPort, szPolicy);

					if (strcmp(szPipe, "INPUT") == 0)
					{
						printf("<%s>\n", szType);
						printf("<PIPE>%s</PIPE>\n", szPipe);
						printf("<DIR>%s</DIR>\n", szDir);
						printf("<IF>%s</IF>\n", szInterface);
						printf("<SIP>%s</SIP>\n", szSIP);
						printf("<DIP>%s</DIP>\n", szDIP);
						printf("<PROTO>%s</PROTO>\n", szProto);
						printf("<SPORT>%s</SPORT>\n", szSPort);
						printf("<DPORT>%s</DPORT>\n", szDPort);
						printf("<POLICY>%s</POLICY>\n", szPolicy);
						printf("</%s>\n", szType);
					}
				}
			}
			printf("</STATUS_NETWORK>\n");
			pclose(fp);
		}

    }
    else if (strcmp(lpszCmd, "set") == 0)
    {
		int		nRules = 0;
		char	szIF[64] = "ppp0";

		XML_PutHeader(pReq);

		char	*lpszState = pReq->getstr(pReq, "state", false);
		if (lpszState == NULL)
		{
			XML_PutError(pReq, "STATUS_NETFILTER", NULL);
			return	-1;	
		}

		FILE	*fp = fopen("/etc/wan", "r");
		if (fp != NULL)
		{
			fscanf(fp, "%s", szIF);	
			fclose(fp);
		}

		fp = fopen("/tmp/iptables", "w");
		if (fp == NULL)
		{
			XML_PutError(pReq, "SET_NETFILTER", "Can't not create file\n");
			return	-1;
		}
		fprintf(fp, "*filter\n");
		fprintf(fp, ":INPUT ACCEPT\n");
		fprintf(fp, ":FORWARD ACCEPT\n");
		fprintf(fp, ":OUTPUT ACCEPT\n");

		while(1)
		{
			char	*lpszSIP	= pReq->getstrf(pReq, false, "sip%d", nRules);
			char	*lpszProto	= pReq->getstrf(pReq, false, "proto%d", nRules);
			char	*lpszDPort	= pReq->getstrf(pReq, false, "dport%d", nRules);

			if ((lpszSIP == NULL) && (lpszProto == NULL) && (lpszDPort == NULL))
			{
				break;			
			}

			// -A 명령어로 필터 추가.
			fprintf(fp, "-A INPUT \t");

			// ip 등록, 0.0.0.0이 아니면  -s 명령어로 특정 아이피 등록.
			if (strcmp(lpszSIP, "0.0.0.0") != 0)
			{
				fprintf(fp, "-s %s\t", lpszSIP);
			}
	
			// -p 명령어로 프로토콜 등록.
			if (strcmp("all", lpszProto) == 0)
			{
				fprintf(fp, "-p %s\t", lpszProto); 
			}
			else if (strcmp("icmp", lpszProto) == 0)
			{
				fprintf(fp, "-p %s --icmp-type echo-request\t", lpszProto);
			}
			else
			{
				fprintf(fp, "-p %s --dport %s\t", lpszProto, lpszDPort);
			}
	
			fprintf(fp, "-j DROP\n");
			nRules++;
		}

		fprintf(fp, "-A OUTPUT  -o %s -p icmp --icmp-type echo-reply -j ACCEPT\n", szIF);
		fprintf(fp, "-A FORWARD -i eth0 -j ACCEPT\n");
		fprintf(fp, "-A FORWARD -o eth0 -j ACCEPT\n");
		fprintf(fp, "-A FORWARD -i eth1 -j ACCEPT\n");
		fprintf(fp, "-A FORWARD -o eth1 -j ACCEPT\n");

		fprintf(fp, "COMMIT\n");
		fclose(fp);

		//fp = fopen("/etc/service/iptables;sync;sync", "w");
		fp = fopen("/etc/service/iptables", "w");
		fprintf(fp,"%s", lpszState);
		fclose(fp);

		fp = popen("cp -f /tmp/iptables /etc/iptables;sync;sync", "r");
		pclose(fp);

		XML_PutHeader(pReq);

		fp = popen("service iptables restart", "r");
		if (fp != NULL)
		{
			pclose(fp);	
			XML_PutOK(pReq, "SET_NETFILTER");

		}
		else
		{
			XML_PutError(pReq, "SET_NETFILTER", "Occurred error when iptables updating\n");	
		}

    }
    else
    {
		XML_PutHeader(pReq);
		XML_PutError(pReq, "SET_NETFILTER", "Invalid command\n");	
	}

    return	0;
}

