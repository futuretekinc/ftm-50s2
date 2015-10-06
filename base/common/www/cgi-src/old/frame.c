#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "qdecoder.h"

typedef	struct _COMMAND
{
	char	*lpszName;
	int		(*fService)(qentry_t *req);
} FTMC_COMMAND;

int	FTMC_ConfigNAT(qentry_t *pReq);

FTMC_COMMAND	cmds[] =
{
	{	"config_nat",				FTMC_ConfigNAT			},
	{	NULL,						NULL					}
};

FTMC_COMMAND	*FTMC_GetCmd(char *lpszCmd);

int main(void)
{
    // Parse queries.
    qentry_t *pReq = qcgireq_parse(NULL, 0);

    // Get Mode
    char *lpszCmd = (char *)pReq->getstr(pReq, "frame", false);

    qcgires_setcontenttype(pReq, "text/xml");

	FTMC_COMMAND *pCmd = FTMC_GetCmd(lpszCmd);
	if (pCmd == NULL)
	{
		qcgires_error(pReq, "Invalid query");
	}
	
	pCmd->fService(pReq);
		
    // De-allocate memories
    pReq->free(pReq);
    return 0;
}

FTMC_COMMAND	*FTMC_GetCmd(char *lpszCmd)
{
	FTMC_COMMAND	*pCmd = &cmds[0];

	if (lpszCmd != NULL)
	{
		while(pCmd->lpszName != NULL)
		{
			if (strcmp(pCmd->lpszName, lpszCmd) == 0)
			{
				return	pCmd;	
			}

			pCmd++;
		}
	}

	return	NULL;
}

int	FTMC_ConfigNAT(qentry_t *pReq)
{
	int	ret = 0;
	char	szBuff[4096];
	FILE *fp = fopen("/www/config_nat.html", "r");
	while(fgets(szBuff, sizeof(szBuff), fp))
	{
		printf("%s", szBuff);	
	}
	return	0;

}

