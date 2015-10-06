#include <stdio.h>
#include "serialcom.h"
#define	_POSIX_SOURCE 1
/* Flag indicating datas are ready to be read */
static int flag = 0;

/* SIGIO handler */
void my_handler(int status)
{
	/* Data ready to be read */
	flag = 1;
}

void my_handler_IO(int status, siginfo_t *ioinfo, void *context)
{
	switch(ioinfo->si_code)
	{
	case	POLL_IN:
		flag = 1;
		printf("signal received for info chars.sig:%d - %d\n", status, ioinfo->si_code);
		break;

	case	POLL_OUT:
		printf("signal received for something else.sig:%d - %d\n", status, ioinfo->si_code);
		break;
	}
}

/* MAIN */
int main(int argc, char **argv)
{
	/* Initialize the serial communication */
	int fdesc = serialConfiguration(my_handler, my_handler_IO, "/dev/ttyUSB0", B9600, NO_PARITY_CHECK);

	/* Data buffer */
	char buf;
	/* Reading result */
	int loop, i, res = 0;
	int count = 0;
	/* Initialize data flag */
	flag = 0;
	
	char	cmd[16][2] = {
		{0xFA, 0xF1},
		{0xFA, 0xFF},
		{0xFA, 0xF5},
		{0xFA, 0xFF},
		{0xFA, 0xF2},
		{0xFA, 0xFF},
		{0xFA, 0xF3},
		{0xFA, 0xFF},
		{0xFA, 0xF4},
		{0xFA, 0xFF},
		{0xFA, 0xF6},
		{0xFA, 0xFF},
		{0xFA, 0xF7},
		{0xFA, 0xFF},
		{0xFA, 0xF8},
		{0xFA, 0xFF}};
	char	resp[16];
	int		nresp = 0;

	for(loop = 0 ; loop < 16 ; loop++)
	{
		printf("%02x %02x -> ", cmd[loop][0], cmd[loop][1]);	
		write(fdesc, cmd[loop], 2);
		count = 0;
		nresp = 0;
		while (count < 10)
		{
			usleep(100000);
			count++;
			/* If there is a data to be read */
			if (flag == 1)
			{
				/* Read one byte */
				do
				{
					res = read(fdesc, &resp[nresp], 1);
					if (res != 0)
					{
						nresp++;	
					}
				}
				while (res > 0);

				flag = 0;
				count = 0 ;
			}
		}

		for(i = 0 ; i < nresp ; i++)
		{
			printf("%02x ", resp[i]);	
		}
		printf("\n");
	}
	return 0;
}

