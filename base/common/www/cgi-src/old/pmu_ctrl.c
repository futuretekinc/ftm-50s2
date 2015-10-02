#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <termios.h>
#include "serialcom.h"
#define	_POSIX_SOURCE 1

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

int nWaitFlag=TRUE;                    /* TRUE while no signal received */


static	void	PMU_SignalHandler(int	nStatus);
static	void	PMU_SignalHandlerIO(int	nStatus, siginfo_t *pIOInfo, void *pContext);

void PMUC_SignalHandler(int status)
{
	nWaitFlag = FALSE;
}

void PMUC_SignalIOHandler(int nStatus, siginfo_t *pIOInfo, void *pContext)
{
	switch(pIOInfo->si_code)
	{
	case	POLL_IN:
		nWaitFlag = FALSE;
		break;

	case	POLL_OUT:
		break;
	}
}

int PMUC_Cmd(int nID, int nState)
{
	char	szReq[2];
	char	szResp[16];
	int		nRespLen = 0;
	int		nTimeout;
	int		nRes;
	int		nFD = serialConfiguration(PMUC_SignalHandler, PMUC_SignalIOHandler, MODEMDEVICE, B9600, NO_PARITY_CHECK);

	szReq[0] = 0xFA;
	if (0 <= nID && nID < 4)
	{
		szReq[1] = 0xF1 + nID + nState * 4;
	}
	else	
	{
		szReq[1] = 0xFF;
	}

	write(nFD, szReq, 2);
	nTimeout = 10;
	while(nTimeout > 0)
	{
		nTimeout--;
		usleep(1000);
		if (nWaitFlag == FALSE)
		{
			/* Read one byte */
			do
			{
				nRes = read(nFD, &szResp[nRespLen], 1);
				if (nRes != 0)
				{
					nRespLen++;	
				}
			}
			while (nRes > 0);

			nWaitFlag = TRUE;
			//nTimeout = 10;
		}
	}

	close(nFD);

	if ((nRespLen < 2) || (szResp[0] != 0xAB))
	{
		return	-1;	
	}

	return	(~szResp[1] & 0x0F);
}


