#ifndef __UTIL_H__
#define	__UTIL_H__

typedef enum 
{
	PROC_STATUS_STOPPED = 0,
	PROC_STATUS_RUNNING = 1
} PROC_STATUS;

PROC_STATUS PROC_Status(char *szProc);
int			PROC_Active(char *szProc);
int			PROC_SetActive(char *szProc, int bEnabled);
int			PROC_Restart(char *szProc);

#endif

