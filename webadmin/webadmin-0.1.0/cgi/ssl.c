#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"
#include "debug.h"

int FTMC_SSLPLUS(qentry_t *pReq);
int FTMC_SECUWAYSSL(qentry_t *pReq);

int FTMC_SSL(qentry_t *pReq)
{
	FILE *fp = fopen("/etc/service/sslplus", "r");
	if (fp != NULL)
	{
		char	szBuf[64];

		if ((1 == fscanf(fp, "%s", szBuf)) && (strcmp(szBuf, "enabled") == 0))
		{
			fclose(fp);
			return	FTMC_SSLPLUS(pReq);	
		}
		fclose(fp);
	}

	fp = fopen("/etc/service/secuwayssl", "r");
	if (fp != NULL)
	{
		char	szBuf[64];

		if ((1 == fscanf(fp, "%s", szBuf)) && (strcmp(szBuf, "enabled") == 0))
		{
			fclose(fp);
			return	FTMC_SECUWAYSSL(pReq);	
		}
		fclose(fp);
	}


	qcgires_setcontenttype(pReq, "text/xml");
	printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
	printf("<SSL_CONFIG>\n");
	printf("<RET>FAILED</RET>\n");
	printf("</SSL_CONFIG>\n");

	return	0;
}
