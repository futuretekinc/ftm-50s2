#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "qdecoder.h"
#include "profile.h"
#include "debug.h"
#define	MAGIC		0x27051956
#define	NAME_LEN	32

typedef	struct
{
	uint32_t	uiMagic;
	uint32_t	uiHeaderCRC;
	uint32_t	uiTime;
	uint32_t	uiSize;
	uint32_t	uiLoad;
	uint32_t	uiEP;
	uint32_t	uiDataCRC;
	uint8_t		uiOS;
	uint8_t		uiArch;
	uint8_t		uiType;
	uint8_t		uiComp;
	uint8_t		uiName[NAME_LEN];
} FTMC_IMAGE_HEADER;

int FTMC_LoadIndex(qentry_t *pReq);
void XML_PutOK(qentry_t *pReq, char *lpszCmd);
void XML_PutError(qentry_t *pReq, char *lpszCmd, char *lpszMsg);
int	FTMC_UpgradeFW(char *lpszSrc, char *lpszDest);
int	FTMC_ReadImageHeader(char *lpszDevice, FTMC_IMAGE_HEADER *pxHeader);
int	FTMC_PrintImageHeader(FTMC_IMAGE_HEADER *pxHeader);


int FTMC_System(qentry_t *pReq)
{

	char *lpszCmd = pReq->getstr(pReq, "cmd", false);

	if (strcmp(lpszCmd, "profile") == 0)
	{
		FTMC_PROFILE_CONFIG	xProfile;
		XML_PutHeader(pReq);

		if (FTMC_LoadProfileConfig(&xProfile) < 0)
		{
			XML_PutError(pReq, "PROFILE", "Failed to load profile data.");
			return	-1;	
		}
		else
		{
			printf("<SYSTEM_INFO>\n");
			printf("<PROFILE>\n");
			printf("<MODEL>%s</MODEL>\n", xProfile.szModel);
			printf("<SN>%s</SN>\n", xProfile.szSerialNumber);
			printf("<LOCATION>%s</LOCATION>\n", xProfile.szLocation);
			printf("</PROFILE>\n");
			printf("</SYSTEM_INFO>\n");
		}
	}
	else if (strcmp(lpszCmd, "chg_loc") == 0)
	{
		FTMC_PROFILE_CONFIG	xProfile;

		XML_PutHeader(pReq);

		if (FTMC_LoadProfileConfig(&xProfile) < 0)
		{
			XML_PutError(pReq, "PROFILE", "Failed to load profile data.");
			return	-1;	
		}
		else
		{
			char *lpszLoc = pReq->getstr(pReq, "loc", false);

			if ((lpszLoc == NULL) || (strlen(lpszLoc) > MAX_LOCATION_LEN))
			{
				XML_PutError(pReq, "SYSTEM", "Invalid location length.");
				return	-1;	
			}

			strcpy(xProfile.szLocation, lpszLoc);

			if (FTMC_SaveProfileConfig(&xProfile) < 0)
			{
				XML_PutError(pReq, "SYSTEM", "Failed to save profile data.");
				return	-1;	
			}

			XML_PutOK(pReq, "SYSTEM");
		}
			
	}
	else if (strcmp(lpszCmd, "chg_passwd") == 0)
	{
		FTMC_PROFILE_CONFIG	xProfile;

		XML_PutHeader(pReq);

		if (FTMC_LoadProfileConfig(&xProfile) < 0)
		{
			XML_PutError(pReq, "PROFILE", "Failed to load profile data.");
			return	-1;	
		}
		else
		{
			char *lpszPasswd = pReq->getstr(pReq, "passwd", false);
			char *lpszNewPasswd = pReq->getstr(pReq, "new_passwd", false);

			if ((lpszPasswd == NULL) || (lpszNewPasswd == NULL) || 
			    (strlen(lpszPasswd) > MAX_PW_LEN) || strcmp(xProfile.szPasswd, lpszPasswd) != 0)
			{
				XML_PutError(pReq, "SYSTEM", "Invalid password.");
				return	-1;	
			}

			strcpy(xProfile.szPasswd, lpszNewPasswd);

			if (FTMC_SaveProfileConfig(&xProfile) < 0)
			{
				XML_PutError(pReq, "SYSTEM", "Failed to save profile data.");
				return	-1;	
			}

			XML_PutOK(pReq, "SYSTEM");
		}
			
	}
	else if (strcmp(lpszCmd, "fw_info") == 0)
	{
		FTMC_IMAGE_HEADER	xHeader;
		char				szBuf[1024];	
		FILE				*fp;

		XML_PutHeader(pReq);
		printf("<SYSTEM>\n");

		fp = popen("cat /proc/mtd", "r");
		while(fgets(szBuf, sizeof(szBuf), fp) != 0)
		{
			char	szDev[64];
			char	szDevPath[256];

			if ((sscanf(szBuf, "%s", szDev) != 1) || (strncmp(szDev, "mtd", 3) != 0))
			{
				continue;	
			}

			szDev[strlen(szDev) - 1] = 0;

			sprintf(szDevPath, "/dev/%sro", szDev);

			if (FTMC_ReadImageHeader(szDevPath, &xHeader) == 0)
			{
				FTMC_PrintImageHeader(&xHeader);	
			}
		}
		pclose(fp);
		
		printf("</SYSTEM>\n");
	
	}
	else if (strcmp(lpszCmd, "fw_upload") == 0)
	{
		// get queries
		char *lpszData		= pReq->getstr(pReq, "file", false);
		int  nLength		= pReq->getint(pReq, "file.length");
		char *lpszFileName	= pReq->getstr(pReq, "file.filename", false);
		int	 nWriteLen;

		XML_PutHeader(pReq);

		// check queries
		if ((lpszData == NULL) || (lpszFileName == NULL) || (nLength == 0))
		{
			XML_PutError(pReq, "SYSTEM", "Select file, please.");
			return	-1;
		}

		FILE *fp = fopen("/tmp/new_fw", "w");
		if (fp == NULL)
		{
			XML_PutError(pReq, "SYSTEM", "A problem occurred while saving the file.");
			return	-1;
		}

		nWriteLen = fwrite(lpszData, 1, nLength, fp);
		fclose(fp);	
		
		if (nWriteLen != nLength)
		{
			XML_PutError(pReq, "SYSTEM", "A problem occurred while saving the file.");
			return	-1;
		}

		FTMC_IMAGE_HEADER	xHeader;

		if (FTMC_ReadImageHeader("/tmp/new_fw", &xHeader) != 0)
		{
			XML_PutError(pReq, "SYSTEM", "Invalid firmware.");
			return	-1;
		}
		
		printf("<SYSTEM>\n");

		FTMC_PrintImageHeader(&xHeader);
		
		printf("<RET>OK</RET>\n");
		printf("</SYSTEM>\n");

		return 0;

	}
	else if(strcmp(lpszCmd, "fw_upgrade") == 0)
	{
		char	*lpszFileName = pReq->getstr(pReq,"fn",false);
		if (lpszFileName != NULL)
		{
			//int	pid = fork();

			//if (pid < 0)
			//{
//
//			}
//			else if (pid == 0)
//			{
				FILE *fwFp;
				char fwBuf[256];
				if (strncmp(lpszFileName, "UBOOT", 5) == 0)
				{
						//execl("/bin/fwmng", "/bin/fwmng", "-i", "/tmp/new_fw", "-o", "/dev/mtdblock1", NULL);
						//fwFp = popen ("fwmng -i /tmp/new_fw -o /dev/mtdblock1", "r");
				}
				else if (strncmp(lpszFileName, "kernel", 5) == 0)
				{
						//execl("/bin/fwmng", "/bin/fwmng", "-i", "/tmp/new_fw", "-o", "/dev/mtdblock3", NULL);
						fwFp = popen ("./update_kernel.sh", "r");
						while (fgets(fwBuf, sizeof(fwBuf), fwFp))
						{
								fwBuf[strlen(fwBuf)-1] = 0;
								if (strcmp(fwBuf, "done") == 0)
								{
									XML_PutHeader(pReq);
									printf("<SYSTEM>\n");
									printf("<RET>OK</RET>\n");
									printf("</SYSTEM>\n");
								}
						}
				}
				else if (strncmp(lpszFileName, "ramdisk", 7) == 0)
				{
						//execl("/bin/fwmng", "/bin/fwmng", "-i", "/tmp/new_fw", "-o", "/dev/mtdblock5", NULL);
						fwFp = popen ("./update_ramdisk.sh", "r");
						while (fgets(fwBuf, sizeof(fwBuf), fwFp))
						{
								fwBuf[strlen(fwBuf)-1] = 0;
								if (strcmp(fwBuf, "done") == 0)
								{
										XML_PutHeader(pReq);
										printf("<SYSTEM>\n");
										printf("<RET>OK</RET>\n");
										printf("</SYSTEM>\n");
								}
						}


				}
				//			}
				//			else 
				//			{
				//				XML_PutHeader(pReq);
//				XML_PutOK(pReq, "SYSTEM");
//			}
		}
	}
	else if (strcmp(lpszCmd, "fw_progress") == 0)
	{
		char	szProgress[128];

		FILE *fp = popen("/bin/fwmng -s", "r");
		fscanf(fp, "%s", szProgress);
		pclose(fp);
		
		XML_PutHeader(pReq);
		printf("<SYSTEM>\n");
		printf("<PROGRESS>%s</PROGRESS>\n", szProgress);
		printf("<RET>OK</RET>\n");
		printf("</SYSTEM>\n");

	}
	else if(strcmp(lpszCmd, "reboot") == 0)
	{
		/*FTMC_LoadIndex(pReq);
		XML_PutHeader(pReq);

		
		FILE *sumFP = popen("/www/cgi-bin/scripts/sum_data.sh", "r");
		if (sumFP != NULL)
		{
				pclose(sumFP);
		}

		//FILE *fp = popen("sync;sync;", "r");
		FILE *detachFp = popen("echo AT#SHDN > /dev/ttyS1; sleep 0.1", "r");
		if (detachFp != NULL)
		{
				pclose(detachFp);
				XML_PutOK(pReq, "SYSTEM");
		}
		else
		{
				XML_PutError(pReq, "SYSTEM", "");
		}
		//sleep (1);
		

		FILE *fp = popen("sleep 4;sync;sync; reboot", "r");
		if (fp != NULL)
		{
			pclose(fp);	

			//XML_PutOK(pReq, "SYSTEM");
		}
		*/
		//else
		//{
		//	XML_PutError(pReq, "SYSTEM", "");
		//}

			FILE *fp;

			fp = popen("./reboot.sh", "r");
			if (fp == NULL)
			{
					perror("error");
			} else {
					pclose(fp);
			}
			qcgires_setcontenttype(pReq, "text/html");
			printf("Please try again in a few minutes.");
			pReq->free(pReq);
	}
	else
	{
		return	-1;	
	}

	return	0;
}

int	FTMC_UpgradeFW(char *lpszSrc, char *lpszDest)
{
	char	szBuf[4096];
	int		nSize = 0;
	int hSrc = open(lpszSrc, O_RDONLY, O_NONBLOCK);
	int hDest= open(lpszDest, O_WRONLY, O_NONBLOCK);

	if (hSrc < 0)
	{
		XERROR("Source file open failed[%s]\n", lpszSrc);
		close(hDest);
		return	-1;	
	}

	if (hDest < 0)
	{
		XERROR("Destination file open failed[%s]\n", lpszDest);
		close(hSrc);
		return	-1;	
	}

	while((nSize = read(hSrc, szBuf, sizeof(szBuf))) > 0)
	{
		write(hDest, szBuf, nSize);
	}

	close(hDest);
	close(hSrc);

	XTRACE("Firmware upgrade done.\n");
	return	0;
}	

int	FTMC_ReadImageHeader(char *lpszDevice, FTMC_IMAGE_HEADER *pxHeader)
{
	FILE *fp = fopen(lpszDevice, "r");
	if (fp == 0)
	{
		return	-1;	
	}

	int nReadSize = fread(pxHeader, 1, sizeof(FTMC_IMAGE_HEADER), fp);
	fclose(fp);

		
	if (nReadSize != sizeof(FTMC_IMAGE_HEADER))
	{
		return	-1;	
	}
	
	if (htonl(pxHeader->uiMagic) != MAGIC)
	{
		return	-1;
	}

	return	0;
}

int	FTMC_PrintImageHeader(FTMC_IMAGE_HEADER *pxHeader)
{
	char	szName[NAME_LEN+1];
	memset(szName, 0, sizeof(szName));
	strncpy(szName, (char *)pxHeader->uiName, NAME_LEN);

	printf("<IMG_INFO>\n");
	if (strcmp(szName, "XLOADER") == 0)
	{
		printf("<TYPE>XLOADER</TYPE>\n");
	}
	else if (strncmp(szName, "U-Boot", 6) == 0)
	{
		printf("<TYPE>UBOOT</TYPE>\n");
	}
	else if (strncmp(szName, "Linux", 5) == 0)
	{
		printf("<TYPE>KERNEL</TYPE>\n");
	}
	else if (strncmp(szName, "Ramdisk", 5) == 0)
	{
		printf("<TYPE>RAMDISK</TYPE>\n");
	}
	printf("<TIME>%d</TIME>\n", htonl(pxHeader->uiTime));
	printf("<SIZE>%d</SIZE>\n", htonl(pxHeader->uiSize));
	printf("<DESC>%s</DESC>\n", szName);
	printf("</IMG_INFO>\n");

	return	0;
}

