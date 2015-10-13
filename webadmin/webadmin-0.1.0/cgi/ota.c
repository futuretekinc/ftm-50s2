#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "qdecoder.h"
#include "debug.h"
#include "util.h"
#define MODEM "/dev/ttyS1"
#define BUFSIZE 255
#define BAUDRATE B115200
#define FALSE 0
#define TRUE 1
volatile int STOP=FALSE;
int open_port(char *port)
{
		struct termios options;
		int fd;
		fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd == -1)
		{
				printf("open_port: Unable to open the port - ");
		}
		else
		{
				printf ( "Port %s with file descriptor=%i",port, fd);
				fcntl(fd, F_SETFL, FNDELAY);
				tcgetattr( fd, &options );
				cfsetispeed( &options, BAUDRATE );
				cfsetospeed( &options, BAUDRATE );
				options.c_cflag |= ( CLOCAL | CREAD);
				options.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CSIZE);
				options.c_cflag |= CS8;
				options.c_cflag &= ~CRTSCTS;
				options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
				options.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | INLCR | IGNCR);
				options.c_oflag &= ~OPOST;
				if ( tcsetattr( fd, TCSANOW, &options ) == -1 )
						printf ("Error with tcsetattr = %s\n", strerror ( errno ) );
				else		
						printf ( "%s\n", "succeed" );
		}
		return (fd);
}

int FTMC_OTA(qentry_t *pReq)
{
		char *lpszCmd = pReq->getstr(pReq, "cmd", false);
		
		int res;
		int serialFD = open_port(MODEM);
		char buf[BUFSIZE];
		memset(buf,0,BUFSIZE);

		FILE *fp;

		if (strcmp(lpszCmd, "status") == 0)
		{
			//write(serialFD, "AT+CRSM=178,28480,1,4,0", strlen("AT+CRSM=178,28480,1,4,0"));
		}
		else if (strcmp(lpszCmd, "ota") == 0)
		{
			//int res;
			//int serialFD = open_port(MODEM);
			//char buf[BUFSIZE];
			//memset(buf,0,BUFSIZE);
			write(serialFD, "AT#COMS=00\r" , strlen("AT#COMS=00\r"));
			//sleep(60);
			while (STOP==FALSE) {
					res = read(serialFD,buf,255);
					buf[res]=0;
					//printf("test = %s", buf);
					//printf(":%s:%d\n", buf, res);
					if (strstr(buf, "#105") != NULL) {
							STOP=TRUE;
							//=========== OTA 인증 성공 ===========
							qcgires_setcontenttype(pReq, "text/xml");
							printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");	
							printf("<OTA_REQUEST>\n");
							printf("<RESULT>SUCCESS</RESULT>\n");
							printf("</OTA_REQUEST>\n");
					} else if (strstr(buf, "#308:3") != NULL) {
							STOP=TRUE;
							//=========== OTA 중단(IMEI 등록이 안된 USIM) =============
							qcgires_setcontenttype(pReq, "text/xml");
							printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
							printf("<OTA_REQUEST>\n");
							printf("<RESULT>STOP</RESULT>\n");
							printf("</OTA_REQUEST>\n");
					} else if (strstr(buf, "#308:7") != NULL) {
							STOP=TRUE;
							//=========== OTA 다시시도 =============
							qcgires_setcontenttype(pReq, "text/xml");
							printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
							printf("<OTA_REQUEST>\n");
							printf("<RESULT>RESTART</RESULT>\n");
							printf("</OTA_REQUEST>\n");
					}
			}
			//read( serialFD, buf, BUFSIZE );
			//printf("The string is: %s", buf);
			close(serialFD);
			return 0;

		}
		else if (strcmp(lpszCmd, "pppd") == 0)
		{
				fp = popen("killall mmon", "r");
				if (fp != NULL)
				{       
						pclose(fp);                
				}

				fp = popen("service pppd stop", "r");
				if (fp != NULL)
				{
						pclose(fp);
						qcgires_setcontenttype(pReq, "text/xml");
						printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
						printf("<OTA_REQUEST>\n");
						printf("<RESULT>OK</RESULT>\n");
						printf("</OTA_REQUEST>\n");
				}
		}
		else if (strcmp(lpszCmd, "modem") == 0)
		{
				fp = popen("service mdmctrl restart", "r");
				if (fp != NULL)
				{
						pclose(fp);
						qcgires_setcontenttype(pReq, "text/xml");
						printf("<?xml version='1.0' encoding='ISO-8859-1'?>\n");
						printf("<OTA_REQUEST>\n");
						printf("<RESULT>OK</RESULT>\n");
						printf("</OTA_REQUEST>\n");
				}
		}
		else
		{
			//printf("<OTA_REQUEST>\n");
			//printf("<RESULT>FAIL</RESULT>\n");
			//printf("</OTA_REQUEST>\n");
		}
		return 0;
}		
