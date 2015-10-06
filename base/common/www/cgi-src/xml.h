#ifndef	__XML_H__
#define	__XML_H__

#include "qdecoder.h"

void XML_PutHeader(qentry_t *pReq);
void XML_PutError(qentry_t *pReq, char *lpszCmd, char *lpszMsg);
void XML_PutOK(qentry_t *pReq, char *lpszCmd);

#endif

