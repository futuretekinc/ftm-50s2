#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "qdecoder.h"
#include "debug.h"

static int		g_bIsRunning = 0;
static int		g_bIsEnabled = 0;
static char	g_szUserID[256] = {0,};
static char	g_szUserPW[256] = {0,};
static char	g_szVPNIP[256] = {0,};
static char	g_szVPNPort[256] = {0,};
static char	g_szCrypto[256] = {0,};
static char	g_szLogSize[256] = {0,};
static char	g_szVersion3[256] = {0,};
static char	g_szSTS[256] = {0,};

static int FTMC_LoadConfig(void)
{
	char	szBuf[1024];
	char	szName[1024];
	char	szContent[1024];

	FILE *fp = popen("/usr/bin/service ssld status", "r");
	fscanf(fp, "%s", szBuf);
	pclose(fp);

	if (strcmp(szBuf, "running") == 0)
	{
		g_bIsRunning = 1;
	}
	else
	{
		g_bIsRunning = 0;
	}

	fp = fopen("/etc/service/ssld", "r");
	fscanf(fp, "%s", szBuf);
	fclose(fp);

	if (strcmp(szBuf, "enabled") == 0)
	{
		g_bIsEnabled = 1;
	}
	else
	{
		g_bIsEnabled = 0;
	}

	fp = fopen("/usr/local/secuwayssl/conf/client.info", "r");
	if (fp != NULL)
	{
		while(fgets(szBuf, sizeof(szBuf), fp))
		{
			if ((szBuf[0] == '#') || (sscanf(szBuf, "%s %s", szName, szContent) != 2))
			{
				continue;	
			}

			if (strcmp(szName, "userid:") == 0)
			{
				strcpy(g_szUserID, szContent);
			}
			else if (strcmp(szName, "userpw:") == 0)
			{
				strcpy(g_szUserPW, szContent);
			}
			else if (strcmp(szName, "vpn_ip:") == 0)
			{
				strcpy(g_szVPNIP, szContent);
			}
			else if (strcmp(szName, "vpn_port:") == 0)
			{
				strcpy(g_szVPNPort, szContent);
			}
			else if (strcmp(szName, "crypto:") == 0)
			{
				strcpy(g_szCrypto, szContent);
			}
			else if (strcmp(szName, "log_size:") == 0)
			{
				strcpy(g_szLogSize, szContent);
			}
			else if (strcmp(szName, "version3:") == 0)
			{
				strcpy(g_szVersion3, szContent);
			}
			else if (strcmp(szName, "site-to-site:") == 0)
			{
				strcpy(g_szSTS, szContent);
			}
		}
		fclose(fp);
	}

	return	0;
}

int FTMC_SECUWAYSSL(qentry_t *pReq)
{

	char *lpszCmd = pReq->getstr(pReq, "cmd", false);

	if (strcmp(lpszCmd, "status") == 0)
	{

		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<SSL_CONFIG>\n");


		FTMC_LoadConfig();
		printf("<PRODUCT>SecuwaySSL</PRODUCT>\n");
		printf("<MANUFACTURER>Secuwiz</MANUFACTURER>\n");

		if (g_bIsRunning)
		{
			printf("<STATUS>running</STATUS>\n");	
		}
		else
		{
			printf("<STATUS>stopped</STATUS>\n");	
		}

		if (g_bIsEnabled)
		{
			printf("<ENABLED>true</ENABLED>\n");	
		}
		else
		{
			printf("<ENABLED>false</ENABLED>\n");	
		}

		printf("<USERID>%s</USERID>\n", g_szUserID);	
		printf("<PASSWD>%s</PASSWD>\n", g_szUserPW);	
		printf("<VPN_IP>%s</VPN_IP>\n", g_szVPNIP);	
		printf("<VPN_PORT>%s</VPN_PORT>\n", g_szVPNPort);	
		printf("<CRYPTO>%s</CRYPTO>\n", g_szCrypto);	
		printf("<LOG_SIZE>%s</LOG_SIZE>\n", g_szLogSize);	
		printf("<VERSION3>%s</VERSION3>\n", g_szVersion3);	
		printf("<STS>%s</STS>\n", g_szSTS);	
		printf("<RET>OK</RET>\n");
		printf("</SSL_CONFIG>\n");

	}
	else if (strcmp(lpszCmd, "set") == 0)
	{
		int		bChanged = 0;
		int		bRestart = 0;
		char	*lpszEnabled= pReq->getstr(pReq, "enable", false);
		char	*lpszUserId = pReq->getstr(pReq, "userid", false);
		char	*lpszPasswd = pReq->getstr(pReq, "passwd", false);
		char	*lpszIP		= pReq->getstr(pReq, "vpn_ip", false);
		char	*lpszPort	= pReq->getstr(pReq, "vpn_port", false);
		char	*lpszCrypto	= pReq->getstr(pReq, "crypto", false);
		char	*lpszLogSize = pReq->getstr(pReq, "log_size", false);
		char	*lpszVersion3 = pReq->getstr(pReq, "version3", false);
		char	*lpszSTS	= pReq->getstr(pReq, "sts", false);

		qcgires_setcontenttype(pReq, "text/xml");
		printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
		printf("<SSL_CONFIG>\n");

		FTMC_LoadConfig();

		if (strcmp(lpszEnabled, "true") == 0)
		{
			if (!g_bIsEnabled)
			{
				FILE *fp = popen("echo enabled > /etc/service/ssld;sync;sync", "r");
				pclose(fp);

				bRestart = 1;
			}
		}
		else
		{
			if (g_bIsEnabled)
			{
				FILE *fp = popen("echo disabled > /etc/service/ssld;sync;sync", "r");
				pclose(fp);

				bRestart = 1;
			}
		}

		if (strcmp(g_szUserID, lpszUserId) != 0)
		{
			bChanged = 1;	
		}

		if (strcmp(g_szVPNIP, lpszIP) != 0)
		{
			bChanged = 1;	
		}

		if (strcmp(g_szVPNPort, lpszPort) != 0)
		{
			bChanged = 1;	
		}

		if (strcmp(g_szCrypto, lpszCrypto) != 0)
		{
			bChanged = 1;	
		}

		if (strcmp(g_szLogSize, lpszLogSize) != 0)
		{
			bChanged = 1;	
		}

		if (strcmp(g_szVersion3, lpszVersion3) != 0)
		{
			bChanged = 1;	
		}

		if (strcmp(g_szSTS, lpszSTS) != 0)
		{
			bChanged = 1;	
		}

		if (strlen(lpszPasswd) == 0)
		{
			lpszPasswd = g_szUserPW;	
		}

		if (strcmp(g_szUserPW, lpszPasswd) != 0)
		{
			bChanged = 1;	
		}

		if (bChanged)
		{
			char	szBuf[1024];
			FILE *fp = fopen("/tmp/client.info", "w");
			if (fp != NULL)
			{
				fprintf(fp, "userid: %s\n", lpszUserId);
				fprintf(fp, "userpw: %s\n", lpszPasswd);
				fprintf(fp, "vpn_ip: %s\n", lpszIP);
				fprintf(fp, "vpn_port: %s\n", lpszPort);
				fprintf(fp, "crypto: %s\n", lpszCrypto);
				fprintf(fp, "log_size: %s\n", lpszLogSize);
				fprintf(fp, "version3: %s\n", lpszVersion3);
				fprintf(fp, "site-to-site: %s\n", lpszSTS);
				fclose(fp);
				
				fp = popen("/bin/cp -f /tmp/client.info /usr/local/secuwayssl/conf/;/bin/rm /tmp/client.info; sync; sync", "r");
				fscanf(fp, "%s", szBuf);
				pclose(fp);

			}

			bRestart = 1;
		}

		if (bRestart)
		{
			FILE *fp = popen("/usr/bin/service ssld restart &", "r");
			pclose(fp);
		}

		printf("<RET>OK</RET>\n");
		printf("</SSL_CONFIG>\n");
	}
	else if (strcmp(lpszCmd, "restart") == 0)
	{
	}

	return	0;
}
