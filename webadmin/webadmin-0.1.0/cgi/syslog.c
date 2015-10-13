#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"


int	FTMC_Syslog(qentry_t *pReq)
{
	qcgires_setcontenttype(pReq, "text/xml");
	printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
	printf("<SYSLOG>\n");

    char	*lpszCmd = pReq->getstr(pReq, "cmd", false);
	char	szBuf[1024];

	FILE *pPF = popen("/www/cgi-bin/status_syslog.sh", "r");
	if (pPF != NULL)
	{
		printf("<INFO>\n");
		while(fgets(szBuf, sizeof(szBuf), pPF) != 0)
		{
			int	nCount;
			if (1 == sscanf(szBuf, "%d", &nCount))
			{
				printf("<COUNT>%d</COUNT>\n", nCount);
			}
		}
		pclose(pPF);
		printf("</INFO>\n");
    }

    if (strcmp(lpszCmd, "get") == 0)
    {
		char *lpszIndex = pReq->getstr(pReq, "index", false);
		char *lpszCount = pReq->getstr(pReq, "count", false);
   
		if ((lpszIndex != NULL) && (lpszCount != NULL))
		{
			int	nIndex = atoi(lpszIndex);	
			int	nCount = atoi(lpszCount);	
			int nOutputCount = 0;

			char	szBuf[1024];
			char	szCmd[1024];

			sprintf(szCmd, "/www/cgi-bin/get_syslog.sh %d %d", nIndex, nCount);
			FILE *fp = popen(szCmd, "r");
			if (fp != NULL)
			{
				while(fgets(szBuf, sizeof(szBuf), fp) != NULL)
				{
					char	*lpszBuf = szBuf;
					char	szTime[16] = {0, };	
					char	szDevice[64] = {0, };	
					char	szAuth[64] = {0, };
					char	szProcess[64] = {0, };	
					char	*lpszContent = NULL;
					char	*lpszTmp = NULL;

					strncpy(szTime, lpszBuf, sizeof(szTime) - 1);
					lpszBuf += strlen(szTime);

					if (3 == sscanf(lpszBuf, "%s %s %s", szDevice, szAuth, szProcess))
					{
						lpszContent = strstr(lpszBuf, szProcess) + strlen(szProcess);
						while((*lpszContent != 0) && isspace(*lpszContent))
						{
							lpszContent++;	
						}
					}

					if ((lpszTmp = strchr(szProcess, '[')) != NULL)
					{
						*lpszTmp = 0;
					}

					if ((lpszTmp = strchr(szProcess, ':')) != NULL)
					{
						*lpszTmp = 0;
					}
					printf("<ENTITY>\n");	
					printf("<TIME>%s</TIME>\n", szTime);
					printf("<PROCESS>%s</PROCESS>\n", szProcess);	
					printf("<LOG>%s</LOG>\n", lpszContent);	
					printf("</ENTITY>\n");	
					nOutputCount++;
				}
			}
		}
    }
	printf("</SYSLOG>\n");

    return	0;
}

