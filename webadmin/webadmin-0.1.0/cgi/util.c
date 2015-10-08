#include <stdlib.h>
#include <string.h>
#include "qdecoder.h"
#include "util.h"


PROC_STATUS PROC_Status(char *szProc)
{
	char	szCmd[1024];
	char	szStatus[16];

	sprintf(szCmd, "/usr/bin/service %s status", szProc);

	FILE *fp = popen(szCmd, "r");
	if (fp != NULL)
	{
		fscanf(fp, "%s", szStatus);
		pclose(fp);

		if (strcmp(szStatus, "running") == 0)
		{
			return	PROC_STATUS_RUNNING;	
		}
	}

	return	PROC_STATUS_STOPPED;	
}

int PROC_Active(char *szProc)
{
	char	szFilePath[1024];
	char	szActive[16];

	sprintf(szFilePath, "/etc/service/%s", szProc);

	FILE *fp = fopen(szFilePath, "r");
	if (fp != NULL)
	{
		fscanf(fp, "%s", szActive);
		fclose(fp);

		if (strcmp(szActive, "enabled") == 0)
		{
			return	1;
		}
	}

	return	0;
}

int PROC_SetActive(char *szProc, int bEnabled)
{
	char	szCmd[1024];
	char	szDummy[1024];

	if (bEnabled)
	{
		sprintf(szCmd, "echo enabled > /etc/service/%s;sync;sync", szProc);
	}
	else
	{
		sprintf(szCmd, "echo disabled > /etc/service/%s;sync;sync", szProc);
	}

	FILE *fp = popen(szCmd, "r");
	fscanf(fp, "%s", szDummy);
	pclose(fp);

	return	0;
}

int	PROC_Restart(char *szProc)
{
	char	szCmd[1024];

	sprintf(szCmd, "/usr/bin/service %s restart &", szProc);
	FILE *fp = popen(szCmd, "r");
	pclose(fp);

	return	1;
}

