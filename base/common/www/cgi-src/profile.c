#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "qdecoder.h"
#include "profile.h"

static char *FTMC_SkipSpace(char *lpszStr)
{
	while(0 != (*lpszStr) && isspace(*lpszStr))
	{
		lpszStr++;
	}

	if (0 != (*lpszStr))
	{
		int nLen = strlen(lpszStr);

		for(int i = nLen - 1 ; i >= 0 ; i--)
		{
			if (!isspace(lpszStr[i]))
			{
				break;
			}

			lpszStr[i] = 0;
		}
	}
	return	lpszStr;
}

int	FTMC_LoadProfileConfig(FTMC_PROFILE_CONFIG *pConfig)
{
	FTMC_PROFILE_CONFIG	xConfig;
	char				szBuf[512];

	memset(&xConfig, 0, sizeof(xConfig));

	FILE *fp = fopen("/etc/httpd.conf", "r");
	if (fp == NULL)
	{
		return	-1;	
	}

	while(0 != fgets(szBuf, sizeof(szBuf), fp))
	{
		char	szTag[32];
		char	*lpszPtr = szBuf;
	
		if (szBuf[0] == '#')
		{	
			continue;
		}

		if (1 != sscanf(lpszPtr, "%s", szTag))
		{
			continue;
		}

		if (0 == strcmp(szTag, "location:"))
		{
			lpszPtr = FTMC_SkipSpace(lpszPtr + strlen(szTag));

			strncpy(xConfig.szLocation, lpszPtr, sizeof(xConfig.szLocation) - 1);
		}
		else if (0 == strcmp(szTag, "userid:"))
		{
			lpszPtr = FTMC_SkipSpace(lpszPtr + strlen(szTag));

			strncpy(xConfig.szUserID, lpszPtr, sizeof(xConfig.szUserID) - 1);
		}
		else if (0 == strcmp(szTag, "passwd:"))
		{
			lpszPtr = FTMC_SkipSpace(lpszPtr + strlen(szTag));

			strncpy(xConfig.szPasswd, lpszPtr, sizeof(xConfig.szPasswd) - 1);
		}
		else if (0 == strcmp(szTag, "timeout:"))
		{
			int	nTimeout;

			sscanf(lpszPtr + strlen(szTag), "%d", &nTimeout);
			lpszPtr = FTMC_SkipSpace(lpszPtr + strlen(szTag));

			if (nTimeout < 0)
			{
				xConfig.nTimeout = nTimeout;	
			}
		}
	}

	fclose(fp);

	fp = fopen("/var/lib/misc/model", "r");
	if (fp != NULL)
	{
		fscanf(fp, "%s", szBuf);	
		fclose(fp);

		strncpy(xConfig.szModel, szBuf, sizeof(xConfig.szModel) - 1);
	}

	fp = fopen("/var/lib/misc/sn", "r");
	if (fp != NULL)
	{
		fscanf(fp, "%s", szBuf);	
		fclose(fp);

		strncpy(xConfig.szSerialNumber, szBuf, sizeof(xConfig.szModel) - 1);
	}

	memcpy(pConfig, &xConfig, sizeof(xConfig));

	return	0;
}

int	FTMC_SaveProfileConfig(FTMC_PROFILE_CONFIG *pConfig)
{
	FILE *fp = fopen("/tmp/httpd.conf", "w");

	if (fp == NULL)
	{
		return	-1;	
	}

	fprintf(fp, "location: %s\n", pConfig->szLocation);
	fprintf(fp, "userid: %s\n", pConfig->szUserID);
	fprintf(fp, "passwd: %s\n", pConfig->szPasswd);
	fprintf(fp, "timeout: %d\n", pConfig->nTimeout);
	fclose(fp);

	fp = popen("cp -f /tmp/httpd.conf /etc/httpd.conf", "r");
	pclose(fp);

	return	0;
}

