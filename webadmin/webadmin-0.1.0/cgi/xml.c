#include "xml.h"

void XML_PutHeader(qentry_t *pReq)
{
	qcgires_setcontenttype(pReq, "text/xml");
	printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
}

void XML_PutError(qentry_t *pReq, char *lpszCmd, char *lpszMsg)
{
	printf("<%s>\n", lpszCmd);
	printf("<RET>ERROR</RET>\n");
	if (lpszMsg != NULL)
	{
		printf("<MSG>%s</MSG>\n", lpszMsg);
	}
	printf("</%s>\n", lpszCmd);
}

void XML_PutOK(qentry_t *pReq, char *lpszCmd)
{
	printf("<%s>\n", lpszCmd);
	printf("<RET>OK</RET>\n");
	printf("</%s>\n", lpszCmd);

}
