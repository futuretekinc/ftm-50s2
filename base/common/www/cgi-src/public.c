#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "qdecoder.h"
#include "profile.h"
#include "ssid.h"
#include "md5.h"
#include "util.h"
#include <time.h>

int FTMC_Public(qentry_t *pReq)
{
	char	*lpszCmd	= (char *)pReq->getstr(pReq, "cmd", false);

	if (lpszCmd != NULL)
	{
		XML_PutHeader(pReq);

		if (strcmp(lpszCmd, "get_dev_info") == 0)
		{
			FTMC_PROFILE_CONFIG	xProfile;

			if (FTMC_LoadProfileConfig(&xProfile) < 0)
			{
				XML_PutError(pReq, "PROFILE", "Failed to load profile data.");
				return	-1;	
			}

			printf("<SYSTEM_INFO>\n");
			printf("<PROFILE>\n");
			printf("<MODEL>%s</MODEL>\n", xProfile.szModel);
			printf("<SN>%s</SN>\n", xProfile.szSerialNumber);
			printf("<LOCATION>%s</LOCATION>\n", xProfile.szLocation);
			printf("<TIMEOUT>%d</TIMEOUT>\n", xProfile.nTimeout);
			printf("</PROFILE>\n");
			printf("</SYSTEM_INFO>\n");
		}
		else if (strcmp(lpszCmd, "get_ssid") == 0)
		{
			FTMC_PROFILE_CONFIG xConfig;
			char	*lpszUserID = (char *)pReq->getstr(pReq, "id", false);
			char	*lpszPasswd = (char *)pReq->getstr(pReq, "pw", false);
			char    *lpszSeed	= (char *)pReq->getstr(pReq, "seed", false);

			if (FTMC_LoadProfileConfig(&xConfig) < 0)
			{
				return	-1;	
			}

			printf("<FTM-50>\n");

			if ((strcmp(lpszUserID, xConfig.szUserID) != 0) || 
				(strcmp(lpszPasswd, xConfig.szPasswd) != 0) || 
				(lpszSeed == NULL))
			{
				printf("<RET>ERROR</RET>\n");			
			}
			else
			{
				char	szSSID[64];

				FTMC_MakeSSID(lpszSeed, szSSID, sizeof(szSSID));
				FTMC_SaveSSID(szSSID, time(0));

				printf("<SSID>%s</SSID>\n", szSSID);
			}
			printf("</FTM-50>\n");
		}
		return	0;
	}

	return	-1;
}

