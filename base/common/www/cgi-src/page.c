#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"


int	FTMC_Page(qentry_t *pReq)
{
	char	szBuf[1024];
    char	*lpszCmd = pReq->getstr(pReq, "cmd", false);
	char	szInnerFileName[1024];

	qcgires_setcontenttype(pReq, "text/html");

	FILE *fp = fopen("/www/contents/base", "r");
	if (fp == NULL)
	{
		return	-1;	
	}

	while(fgets(szBuf, sizeof(szBuf), fp))
	{
		if (strstr(szBuf, "###SCRIPT###") != 0)
		{
			snprintf(szInnerFileName, sizeof(szInnerFileName), "/www/scripts/%s.js", lpszCmd);
			FILE *ifp = fopen(szInnerFileName, "r");
			if (ifp != NULL)
			{
				printf("<script type='text/javascript'>\n");
				while(fgets(szBuf, sizeof(szBuf), ifp))
				{
					printf(szBuf);	
				}
				printf("</script>\n");
				
				fclose(ifp);
			}
		}
		else if (strstr(szBuf, "###CONTENT###") != 0)
		{
			snprintf(szInnerFileName, sizeof(szInnerFileName), "/www/contents/%s", lpszCmd);
			FILE *ifp = fopen(szInnerFileName, "r");
			if (ifp != NULL)
			{
				while(fgets(szBuf, sizeof(szBuf), ifp))
				{
					printf(szBuf);	
				}
				
				fclose(ifp);
			}
		}
		else
		{
			printf(szBuf);	
		}
	}

	fclose(fp);

    return	0;
}

