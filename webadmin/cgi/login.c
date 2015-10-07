#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "qdecoder.h"

static int FTMC_PrintInnerContent(char *strFileName)
{
	FILE	*pFP = fopen(strFileName, "rt");
	char	szBuf[1024];	

	if (pFP == NULL)
	{
		return	-1;	
	}

	while(fgets(szBuf, sizeof(szBuf), pFP))
	{
		printf(szBuf);	
	}

	fclose(pFP);

	return	0;
}

int	FTMC_Main(qentry_t *pReq)
{
	char	szBuf[1024];
	FILE	*fp = fopen("../contents/base", "rt");
	if (fp == NULL)
	{
		return	-1;	
	}

    qcgires_setcontenttype(pReq, "text/html");
	while(fgets(szBuf, sizeof(szBuf), fp))
	{
		printf(szBuf);		
	}

	fclose(fp);
    return	0;
}

