#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"
#include "xml.h"
#include "util.h"
#include "debug.h"

typedef	struct
{
	int			bActive;
	PROC_STATUS	xStatus;
	char		szUserID[256];
	char		szPasswd[256];
	char		szHostIP[256];
}	SSLPLUS_CONFIG;

static int SSLPLUS_LoadConfig(SSLPLUS_CONFIG *pConfig)
{
	char	szBuf[1024];
	char	szName[1024];
	char	szIndicator[128];
	char	szContent[1024];

	pConfig->bActive = PROC_Active("ssld");
	pConfig->xStatus = PROC_Status("ssld");

	FILE *fp = popen("/www/cgi-bin/sslplus_conv /usr/local/frodo/sslplus.conf", "r");
	if (fp == NULL)
	{
		return	-1;
	}

	while(fgets(szBuf, sizeof(szBuf), fp))
	{
		if ((szBuf[0] == '#') || (sscanf(szBuf, "%s %s %s", szName, szIndicator, szContent) != 3))
		{
			continue;	
		}

		if (strcmp(szName, "login_id") == 0)
		{
			strncpy(pConfig->szUserID, szContent, sizeof(pConfig->szUserID) - 1);
		}
		else if (strcmp(szName, "login_pw") == 0)
		{
			strncpy(pConfig->szPasswd, szContent, sizeof(pConfig->szPasswd) - 1);
		}
		else if (strcmp(szName, "host") == 0)
		{
			strncpy(pConfig->szHostIP, szContent, sizeof(pConfig->szHostIP) - 1);
		}
	}
	fclose(fp);

	return	0;

}

static int SSLPLUS_SaveConfig(SSLPLUS_CONFIG *pConfig)
{
	FILE *fp = fopen("/tmp/sslplus.conf", "w");
	if (fp == NULL)
	{
		return	-1;
	}

	fprintf(fp, "{\n");
	fprintf(fp, "\t\"hosts\":[\n");
	fprintf(fp, "\t\t{\n");
	fprintf(fp, "\"host\": \"%s\"\n", pConfig->szHostIP);
	fprintf(fp, "\"description\": \"\"\n");
	fprintf(fp, "\"login_id\": \"%s\"\n", pConfig->szUserID);
	fprintf(fp, "\"login_pw\": \"%s\"\n", pConfig->szPasswd);
	fprintf(fp, "\"login_cacert\": \"/usr/local/frodo/cert/ca.crt\"\n");
	fprintf(fp, "\"login_cert\": \"/usr/local/frodo/cert/client1.crt\"\n");
	fprintf(fp, "\"login_key\": \"/usr/local/frodo/cert/client1.key\"\n");
	fprintf(fp, "\"login_keypw\": \"\"\n");
	fprintf(fp, "\t\t}\n");
	fprintf(fp, "\t]\n");
	fprintf(fp, "}\n");
	fclose(fp);

	fp = popen("/bin/cp -f /tmp/sslplus.conf /usr/local/frodo;rm /tmp/sslplus.conf;sync;sync", "r");
	pclose(fp);

	return	0;
}

static int	SSLPLUS_Status(qentry_t *pReq)
{
	SSLPLUS_CONFIG	xConfig;

	if (SSLPLUS_LoadConfig(&xConfig) < 0)
	{
		XML_PutError(pReq, "SSL_CONFIG", NULL);
		return	-1;
	}

	printf("<SSL_CONFIG>\n");
	printf("<PRODUCT>SSLPlus</PRODUCT>\n");
	printf("<MANUFACTURER>Future System, Inc.</MANUFACTURER>\n");
	printf("<STATUS>%s</STATUS>\n", (xConfig.xStatus == PROC_STATUS_RUNNING)?"running":"stopped");	
	printf("<ACTIVE>%s</ACTIVE>\n", (xConfig.bActive)?"yes":"no");	
	printf("<USERID>%s</USERID>\n", xConfig.szUserID);	
	printf("<PASSWD>%s</PASSWD>\n", xConfig.szPasswd);	
	printf("<HOST>%s</HOST>\n",		xConfig.szHostIP);	
	printf("</SSL_CONFIG>\n");

	return	0;
}

static int	SSLPLUS_Set(qentry_t *pReq)
{
	SSLPLUS_CONFIG	xConfig;
	int		bModified = 0;
	int		bRestart = 0;
	char	*lpszActive	= pReq->getstr(pReq, "active", false);
	char	*lpszUserID = pReq->getstr(pReq, "userid", false);
	char	*lpszPasswd = pReq->getstr(pReq, "passwd", false);
	char	*lpszHostIP	= pReq->getstr(pReq, "host", false);


	if (SSLPLUS_LoadConfig(&xConfig) < 0)
	{
		return	-1;
	}

	if (strcmp(lpszActive, "yes") == 0)
	{
		if (!xConfig.bActive)
		{
			PROC_SetActive("ssld", 1);

			bRestart = 1;
		}
	}
	else
	{
		if (xConfig.bActive)
		{
			PROC_SetActive("ssld", 0);

			bRestart = 1;
		}
	}

	if (strcmp(xConfig.szUserID, lpszUserID) != 0)
	{
		strncpy(xConfig.szUserID, lpszUserID, sizeof(xConfig.szUserID) - 1);
		bModified = 1;	
	}

	if (strlen(lpszPasswd) == 0)
	{
		lpszPasswd = xConfig.szPasswd;	
	}

	if ((strlen(lpszPasswd) != 0) && (strcmp(xConfig.szPasswd, lpszPasswd) != 0))
	{
		strncpy(xConfig.szPasswd, lpszPasswd, sizeof(xConfig.szPasswd) - 1);
		bModified = 1;	
	}

	if (strcmp(xConfig.szHostIP, lpszHostIP) != 0)
	{
		strncpy(xConfig.szHostIP, lpszHostIP, sizeof(xConfig.szHostIP) - 1);
		bModified = 1;	
	}

	if (bModified)
	{
		if (SSLPLUS_SaveConfig(&xConfig) < 0)
		{
			return	-1;
		}
	}

	if (bRestart)
	{
		PROC_Restart("ssld");
	}

	return	0;
}

int FTMC_SSLPLUS(qentry_t *pReq)
{
	char *lpszCmd = pReq->getstr(pReq, "cmd", false);

	XML_PutHeader(pReq);

	if (strcmp(lpszCmd, "status") == 0)
	{
		return	SSLPLUS_Status(pReq);
	}
	else if (strcmp(lpszCmd, "set") == 0)
	{
		if (SSLPLUS_Set(pReq) < 0)
		{
			XML_PutError(pReq, "SSL_CONFIG", NULL);
			return	-1;
		}
		else
		{
			XML_PutOK(pReq, "SSL_CONFIG");
			return	0;
		}
	}
	else if (strcmp(lpszCmd, "restart") == 0)
	{
	}

	return	0;
}
