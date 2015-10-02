#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "qdecoder.h"

int main(void)
{
    // Parse queries.
    char	szModel[256] = {"FTM-SERIES"};
    char	szDevID[256] = {"UNDEFINED",};

    qentry_t *pReq = qcgireq_parse(NULL, 0);

    qcgires_setcontenttype(pReq, "text/xml");

	FILE	*fp = popen("/www/cgi-bin/dev_info.sh", "r");
	if (fp != NULL)
	{
		fgets(szModel, sizeof(szModel), fp) ;
		fgets(szDevID, sizeof(szDevID), fp) ;
	}
	pclose(fp);

	printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
	printf("<DEV_INFO>\n");
	printf("<MODEL>%s</MODEL>\n", szModel);
	printf("<DEVID>%s</DEVID>\n", szDevID);
	printf("</DEV_INFO>\n");

    // De-allocate memories
    pReq->free(pReq);
    return 0;
}

