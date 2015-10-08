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
#include "debug.h"
#include <time.h>

#define	MAX_BUCKETS		100
#define	VALID_TIME		600

key_t xSharedKey = 0x12554422;

typedef struct 
{
	int		bValid;
	char	szTag[32];
	time_t	xExpiredDate;
} SSID;


typedef	struct
{	
	int		nValidTime;
	SSID	xBuckets[MAX_BUCKETS];
}	SSID_POOL;

int			FTMC_UpdateSSID(void);
SSID_POOL	*FTMC_GetPool(void);

int FTMC_SSID(qentry_t *pReq)
{
	FTMC_PROFILE_CONFIG xConfig;
	char	*lpszCmd	= (char *)pReq->getstr(pReq, "cmd", false);
	char	*lpszUserID = (char *)pReq->getstr(pReq, "id", false);
	char	*lpszPasswd = (char *)pReq->getstr(pReq, "pw", false);
	char    *lpszSeed	= (char *)pReq->getstr(pReq, "seed", false);

	if (strcmp(lpszCmd, "get") == 0)
	{
		if (FTMC_LoadProfileConfig(&xConfig) < 0)
		{
			return	-1;	
		}

		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<FTM-50>\n");

		if ((strcmp(lpszUserID, xConfig.szUserID) != 0) || 
			(strcmp(lpszPasswd, xConfig.szPasswd) != 0) || 
			(lpszSeed == NULL))
		{
			printf("<RET>ERROR</RET>\n");			
		}
		else
		{
			char	szTag[64];

			FTMC_MakeSSID(lpszSeed, szTag, sizeof(szTag));
			FTMC_SaveSSID(szTag, time(0));
			printf("<SSID>%s</SSID>\n", szTag);
			printf("<RET>OK</RET>\n");	

		}
		printf("</FTM-50>\n");

		return	0;
	}

    return	-1;
}

void FTMC_MakeSSID(char *lpszSeed, char *lpszTag, int nLen)
{
	char	szBuf[1024];
	MD5_CTX	xContext;

	MD5Init(&xContext);

	MD5Update(&xContext, lpszSeed, strlen(lpszSeed));
	FILE *pPF = popen("uptime", "r");
	if (pPF != NULL)
	{
		fgets(szBuf, sizeof(szBuf), pPF);
		MD5Update(&xContext, szBuf, strlen(szBuf));
		pclose(pPF);
	}

	pPF = popen("ps | tail -n 1", "r");
	if (pPF != NULL)
	{
		fgets(szBuf, sizeof(szBuf), pPF);
		MD5Update(&xContext, szBuf, strlen(szBuf));
		pclose(pPF);
	}

	MD5Final(&xContext);

	sprintf(szBuf, "%08x%08x%08x%08x", 
			((unsigned int *)xContext.digest)[0],
			((unsigned int *)xContext.digest)[1],
			((unsigned int *)xContext.digest)[2],
			((unsigned int *)xContext.digest)[3]);
	memcpy(lpszTag, szBuf, nLen);	
	
}


int FTMC_SaveSSID(char szNewSSID[32], time_t xTime)
{
	int				nOldestSSID = 0;
	unsigned int	uOldestExpiredTime = 0;
	SSID_POOL		*pxPool;

	pxPool = FTMC_GetPool();
	if (pxPool == NULL)
	{
		return	-1;	
	}

	for(int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (pxPool->xBuckets[i].bValid == 1 && memcmp(pxPool->xBuckets[i].szTag, szNewSSID, 32) == 0)
		{
			pxPool->xBuckets[i].xExpiredDate = xTime + pxPool->nValidTime;
			return	0;
		}
	}

	for(int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (pxPool->xBuckets[i].bValid == 0)
		{
			pxPool->xBuckets[i].bValid = 1;
			memcpy(pxPool->xBuckets[i].szTag, szNewSSID, 32);
			pxPool->xBuckets[i].xExpiredDate = xTime + pxPool->nValidTime;
			return	0;
		}

		if ((unsigned int )pxPool->xBuckets[i].xExpiredDate < uOldestExpiredTime)
		{
			uOldestExpiredTime = (unsigned int)pxPool->xBuckets[i].xExpiredDate;
			nOldestSSID = i;
		}
	}

	pxPool->xBuckets[nOldestSSID].bValid = 1;
	memcpy(pxPool->xBuckets[nOldestSSID].szTag, szNewSSID, 32);
	pxPool->xBuckets[nOldestSSID].xExpiredDate = xTime + pxPool->nValidTime;

	return	0;
}

int FTMC_DeleteSSID(char szTag[32])
{
	int			nSharedID;
	SSID_POOL	*pxPool;

	nSharedID = shmget(xSharedKey, sizeof(SSID_POOL), 0666);
	if (nSharedID < 0)
	{
		return	0;
	}

	pxPool = (SSID_POOL *)shmat(nSharedID, NULL, 0);
	if (pxPool == NULL)
	{
		return	0;
	}


	for(int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (pxPool->xBuckets[i].bValid == 1 && memcmp(pxPool->xBuckets[i].szTag, szTag, 32) == 0)
		{
			memset(&pxPool->xBuckets[i], 0, sizeof(pxPool[i]));
			return	0;
		}
	}

	return	0;
}

int FTMC_IsValidSSID(char szTag[32], time_t xTime)
{
	int				nSharedID;
	SSID_POOL		*pxPool;

	nSharedID = shmget(xSharedKey, sizeof(SSID_POOL), 0666);
	if (nSharedID < 0)
	{
		return	0;	
	}

	pxPool = (SSID_POOL *)shmat(nSharedID, NULL, 0);
	if (pxPool == NULL)
	{
		return	0;	
	}

	for(int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (pxPool->xBuckets[i].bValid == 1 && memcmp(pxPool->xBuckets[i].szTag, szTag, 32) == 0)
		{
			int diff = pxPool->xBuckets[i].xExpiredDate - xTime;
			
			if (0 <= diff && diff <= pxPool->nValidTime)
			{
				return	1;
			}
			else
			{
				break;	
			}
		}
	}

	for(int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (pxPool->xBuckets[i].bValid == 1)
		{
			if ((unsigned int)pxPool->xBuckets[i].xExpiredDate - (unsigned int)xTime > pxPool->nValidTime)
			{
				pxPool->xBuckets[i].bValid = 0;
				memset(pxPool->xBuckets[i].szTag, 0, 32);
				pxPool->xBuckets[i].xExpiredDate = 0;
			}
		}
	}

	return	0;
}

int FTMC_TouchSSID(char szTag[32], time_t xTime)
{
	int				nSharedID;
	SSID_POOL		*pxPool;

	nSharedID = shmget(xSharedKey, sizeof(SSID_POOL), 0666);
	if (nSharedID == -1)
	{
		return	0;	
	}

	pxPool = (SSID_POOL *)shmat(nSharedID, NULL, 0);
	if (pxPool == NULL)
	{
		return	0;	
	}

	for(int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (pxPool->xBuckets[i].bValid == 1 && memcmp(pxPool->xBuckets[i].szTag, szTag, 32) == 0)
		{
			pxPool->xBuckets[i].xExpiredDate = (unsigned int)xTime + pxPool->nValidTime;
			return	1;
		}
	}

	return	0;
}

SSID_POOL	*FTMC_GetPool(void)
{
	int				nSharedID, nNew = 0;
	SSID_POOL		*pxPool;

	nSharedID = shmget(xSharedKey, sizeof(SSID_POOL), 0666);
	if (nSharedID < 0)
	{
		nSharedID = shmget(xSharedKey, sizeof(SSID_POOL), 0666 | IPC_CREAT);
		if (nSharedID < 0)
		{
			return	NULL;	
		}
		nNew = 1;
	}

	pxPool = (SSID_POOL *)shmat(nSharedID, NULL, 0);
	if (pxPool == NULL)
	{
		return	NULL;	
	}

	if (nNew)
	{
		memset(pxPool, 0, sizeof(SSID_POOL));	
	}
		pxPool->nValidTime = VALID_TIME;

	return	pxPool;
}

int FTMC_UpdateSSID(void)
{
	int				nSharedID;
	SSID_POOL		*pxPool;
	time_t			xTime = time(0);

	nSharedID = shmget(xSharedKey, sizeof(SSID_POOL), 0666);
	if (nSharedID < 0)
	{
		return	-1;	
	}

	pxPool = (SSID_POOL *)shmat(nSharedID, NULL, 0);
	if (pxPool == NULL)
	{
		return	-1;	
	}

	for(int i = 0 ; i < MAX_BUCKETS ; i++)
	{
		if (pxPool->xBuckets[i].bValid == 1)
		{
			if (pxPool->xBuckets[i].xExpiredDate - xTime > pxPool->nValidTime)
			{
				memset(&pxPool->xBuckets[i], 0, sizeof(pxPool->xBuckets[i]));
			}
		}
	}

	return	0;
}

#if 0
int FTMC_SaveSSID(char *lpszNewSSID, time_t xExpireIn)
{
	char			szBuf[1024];
	int				nIndex;
	char			szTag[64];
	time_t			xTime;

	FILE *pFP = fopen("/var/run/ssid", "r+");
	if (pFP == NULL)
	{
		pFP = fopen("/var/run/ssid", "a+");
		if (pFP == NULL)
		{
			return	-1;	
		}
	}

	while(fgets(szBuf, sizeof(szBuf), pFP))
	{
		if (3 == sscanf(szBuf, "%d %s %u",  &nIndex, szTag, (unsigned int *)&xTime))
		{
			if (strcmp(lpszNewSSID, szTag) == 0)
			{
				fseek(pFP, 0 - strlen(szBuf), SEEK_CUR);	
				break;
			}
		}
	}

	fprintf(pFP, "%s %8u\n", lpszNewSSID, (unsigned int)xExpireIn);
	fclose(pFP);

	return	0;
}

int FTMC_IsValidSSID(char *lpszQureiedSSID, time_t xTime)
{
	char	szBuf[1024];
	char	szTag[64];
	time_t	xExpiredDate;
	int		nRet = 0;

	FILE *pFP = fopen("/var/run/ssid", "r+");
	if (pFP != NULL)
	{
		while(fgets(szBuf, sizeof(szBuf), pFP))
		{
			if (2 == sscanf(szBuf, "%s %u", szTag, (unsigned int *)&xExpiredDate	))
			{
				if ((unsigned int)xExpiredDate - (unsigned int)xTime <= pxPool->nValidTime)	
				{
					if (strcmp(szTag lpszQureiedSSID) == 0)
					{
						nRet = 1;	
					}
				}
				else
				{
					fseek(pFP, 0 - strlen(szBuf), SEEK_CUR);
					fprintf(pFP, "%s %8u\n"
				}
			}
		}

		fclose(pFP);
	}
	return	nRet;
}
#endif
