/******************************************************************************
 * qDecoder - http://www.qdecoder.org
 *
 * Copyright (c) 2000-2012 Seungyoung Kim.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
 * $Id: query.c 636 2012-05-07 23:40:43Z seungyoung.kim $
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ssid.h"
#include "qdecoder.h"

typedef	struct _COMMAND
{
	char	*lpszName;
	int		(*fService)(qentry_t *req);
} FTMC_COMMAND;

int FTMC_Public(qentry_t *pReq);
int FTMC_LoadIndex(qentry_t *pReq);
int	FTMC_SSID(qentry_t *pReq);
int	FTMC_Page(qentry_t *pReq);
int	FTMC_Main(qentry_t *pReq);
int	FTMC_Network(qentry_t *pReq);
int	FTMC_DHCP(qentry_t *pReq);
int FTMC_SSL(qentry_t *pReq);
int FTMC_SSLPLUS(qentry_t *pReq);
int FTMC_SECUWAYSSL(qentry_t *pReq);
int	FTMC_Netfilter(qentry_t *pReq);
int	FTMC_Syslog(qentry_t *pReq);
int	FTMC_System(qentry_t *pReq);
int FTMC_Logout(qentry_t *pReq);
int FTMC_OTA(qentry_t *pReq);

FTMC_COMMAND	cmds[] =
{
	{	"public",			FTMC_Public			},
	{	"page",				FTMC_Page			},
	{	"main",				FTMC_Main			},
	{	"network",			FTMC_Network		},
	{	"dhcp",				FTMC_DHCP			},
	{	"ssl",				FTMC_SSL			},
	{	"sslplus",			FTMC_SSLPLUS		},
	{	"secuwayssl",		FTMC_SECUWAYSSL		},
	{	"firewall",			FTMC_Netfilter		},
	{	"syslog",			FTMC_Syslog			},
	{	"system",			FTMC_System			},
	{	"logout",			FTMC_Logout			},
	{	"ota",				FTMC_OTA			},
	{	NULL,				NULL				}
};

int main(int argc, char *argv[])
{
    // Parse queries.
    qentry_t *pReq = qcgireq_parse(NULL, 0);

	FTMC_COMMAND	*pCmd = &cmds[0];
	while(pCmd->lpszName != NULL)
	{
		if (strcmp(pCmd->lpszName, argv[0]) == 0)
		{
			break;
		}

		pCmd++;
	}

	if (pCmd->lpszName == NULL)
	{
		qcgires_error(pReq, "Not supported command!");
	}
	else	
	{
		char	*lpszSSID = pReq->getstr(pReq, "ssid", false);
		time_t	xTime = time(0);
	
		if (strcmp(pCmd->lpszName, "public") == 0)
		{
			pCmd->fService(pReq);
		}
		else if ((lpszSSID != NULL) && (strlen(lpszSSID) == 32) &&  FTMC_IsValidSSID(lpszSSID, xTime))
		{
			FTMC_SaveSSID(lpszSSID, xTime);
			pCmd->fService(pReq);
		}
		else
		{
			FTMC_LoadIndex(pReq);
		}
	}	

    // De-allocate memories
    pReq->free(pReq);
    return 0;
}

int FTMC_LoadIndex(qentry_t *pReq)
{
	FILE *pFP = fopen("../index.html", "r");
	if (pFP != NULL)
	{
		char	szBuf[1024];

		qcgires_setcontenttype(pReq, "text/html");
		while(fgets(szBuf, sizeof(szBuf), pFP))
		{
			printf(szBuf);	
		}

		fclose(pFP);
	}
	else
	{
		qcgires_error(pReq, "Invalid session !");
	
	}

	return	0;
}

int FTMC_Logout(qentry_t *pReq)
{
	char	*lpszSSID = pReq->getstr(pReq, "ssid", false);
	
	if (lpszSSID != NULL)
	{
		FTMC_DeleteSSID(lpszSSID);	
	}
    qcgires_setcontenttype(pReq, "text/html");
	return	0;
}

