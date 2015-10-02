/******************************************************************************
 * qDecoder - http://www.qdecoder.org
 *
 * Copyright (c) 2000-2012 Seungyoung Kim.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
 * $Id: query.c 636 2012-05-07 23:40:43Z seungyoung.kim $
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "qdecoder.h"
#include "net.h"
#include "pmu_ctrl.h"

#define	CONFIG_MODE_UNKNOWN	0
#define	CONFIG_MODE_FORM	1
#define	CONFIG_MODE_READ	2
#define	CONFIG_MODE_WRITE	3

typedef	struct _CONFIG_CMD
{
	char	*name;
	int		(*form)(qentry_t *req);
	int		(*get)(qentry_t *req);
	int		(*set)(qentry_t *req);
}	CONFIG_CMD;


int	print_split_ip(struct in_addr ip) ;

int	cmd_form_login(qentry_t *req);
int	cmd_set_login(qentry_t *req);
int	cmd_get_status_network(qentry_t *req);
int	cmd_form_summary(qentry_t *req);
int	cmd_get_summary(qentry_t *req);
int	cmd_form_lan(qentry_t *req);
int	cmd_get_lan(qentry_t *req);
int	cmd_set_lan(qentry_t *req);
int	cmd_get_config_dhcp(qentry_t *req);
int	cmd_set_config_dhcp(qentry_t *req);
int cmd_get_static_dhcp_data(qentry_t *req);
int	cmd_form_dhcp_server_config(qentry_t *req);
int	cmd_get_dhcp_server_config(qentry_t *req);
int	cmd_set_dhcp_server_config(qentry_t *req);
int cmd_get_ioctl(qentry_t *req);
int cmd_set_ioctl(qentry_t *req);
int cmd_get_status_syslog(qentry_t *req);

CONFIG_CMD	cmds[] =
{
	{ "login",				cmd_form_login, 			NULL, 							cmd_set_login				},
	{ "status_network",		NULL,						cmd_get_status_network,		NULL						},
	{ "status_syslog",		NULL,						cmd_get_status_syslog,		NULL						},
	{ "config_lan",			cmd_form_lan,				cmd_get_lan,				cmd_set_lan					},
	{ "config_dhcp",		NULL,						cmd_get_config_dhcp,		cmd_set_config_dhcp},
	{ "static_dhcp_data",	NULL,						cmd_get_static_dhcp_data,	NULL						},
	{ NULL,					NULL,						NULL						}
};

int main(void)
{
    // Parse queries.
    qentry_t *req = qcgireq_parse(NULL, 0);

    // Get Mode
    char *mode = (char *)req->getstr(req, "mode", false);
    char *menu = (char *)req->getstr(req, "menu", false);

	if (mode == NULL || menu == NULL)
	{
        qcgires_setcontenttype(req, "text/plain");
        qcgires_error(req, "Invalid query");
		req->free(req);
		return	0;
	}

	int			cmd_mode = CONFIG_MODE_UNKNOWN;

	if (strcmp(mode, "form") == 0)
	{
		cmd_mode = CONFIG_MODE_FORM;	
	}
	else if (strcmp(mode, "read") == 0)
	{
		cmd_mode = CONFIG_MODE_READ;	
	}
	if (strcmp(mode, "write") == 0)
	{
		cmd_mode = CONFIG_MODE_WRITE;	
	}

	CONFIG_CMD *cmd = cmds;
	for(; cmd->name != NULL ; cmd++)
	{
		if (strcmp(menu, cmd->name) == 0)
		{
			switch(cmd_mode)
			{
			case	CONFIG_MODE_FORM:
				if (cmd->get != NULL)
				{
					cmd->get(req);	
				}
				break;
			case	CONFIG_MODE_READ:
				if (cmd->get != NULL)
				{
					cmd->get(req);	
				}
				break;
			case	CONFIG_MODE_WRITE:
				if (cmd->set != NULL)
				{
					cmd->set(req);
				}
				break;

			default:
        		qcgires_setcontenttype(req, "text/plain");
        		qcgires_error(req, "Invalid query");
			}
			break;
		}
	}

	if (cmd->name == NULL)
	{
        qcgires_setcontenttype(req, "text/plain");
        qcgires_error(req, "Invalid query");
	}

    // De-allocate memories
    req->free(req);
    return 0;
}

int	cmd_form_login(qentry_t *req)
{
	return	0;
}

int	cmd_set_login(qentry_t *req)
{
    qcgires_setcontenttype(req, "text/plain");
    char *id= (char *)req->getstr(req, "id", false);
    char *pw= (char *)req->getstr(req, "password", false);

	if ((strcmp(id, "admin") == 0) && (strcmp(pw, "admin") == 0))
	{
		printf("OK|!|12345678");
	}
	else
	{
		printf("Invalid ID or Password!");
	
	}
	return	0;
}

int	cmd_form_summary(qentry_t *req)
{
	FILE *fp;
	char	buf[1024];

    qcgires_setcontenttype(req, "text/plain");

	fp = fopen("../summary.form", "r");
	while(fgets(buf, 1024, fp) != NULL)
	{
		printf(buf);	
	}
	return	0;
}

int	cmd_get_status_network(qentry_t *req)
{
	struct	net_info	info;

	get_net_info("eth0", &info);
    qcgires_setcontenttype(req, "text/plain");
	printf("%s", inet_ntoa(info.ip));	printf("|!|");
	printf("%s", inet_ntoa(info.netmask));	printf("|!|");
	printf("%s", inet_ntoa(info.gateway));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[0]));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[1]));	printf("|!|");

	return	0;
}


int	cmd_get_status_syslog(qentry_t *req)
{
    char	*lpszIDX = (char *)req->getstr(req, "idx", false);
	int		idx = 0;
	time_t	tm;


	if (lpszIDX != NULL)
	{
		idx = atoi(lpszIDX);
	}

	tm = time(NULL);

    qcgires_setcontenttype(req, "text/plain");
	printf("256");				printf("|!|");
	printf("%d", idx*50);		printf("|!|");

	for(int i = 0 ; i < 10 ; i++)
	{
		printf("%d",idx*50+i);		printf("|!|");
		printf("%s",ctime(&tm));	printf("|!|");
		printf("SYS");				printf("|!|");
		printf("XXXXXXXXXX");		printf("|!|");
	}

	return	0;
}

int	cmd_get_summary(qentry_t *req)
{
	struct	net_info	info;

	get_net_info("eth0", &info);
    qcgires_setcontenttype(req, "text/plain");
	printf("FTMB-50");					printf("|!|");
	printf("%s", inet_ntoa(info.ip));	printf("|!|");
	printf("%s", inet_ntoa(info.netmask));	printf("|!|");
	printf("%s", inet_ntoa(info.gateway));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[0]));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[1]));	printf("|!|");

	printf("FIELD"); printf("|!|");
	printf("3600 seconds"); printf("|!|");
	printf("%s", inet_ntoa(info.dns[0]));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[1]));	printf("|!|");

	printf("FTMB-050201308130001"); printf("|!|");
	get_net_info("eth0", &info);
	printf("%s", inet_ntoa(info.ip));	printf("|!|");
	printf("%s", inet_ntoa(info.netmask));	printf("|!|");
	printf("%s", inet_ntoa(info.gateway));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[0]));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[1]));	printf("|!|");
	printf("FIELD"); printf("|!|");
	printf("FIELD"); printf("|!|");
	printf("FIELD"); printf("|!|");

	return	0;
}

int	print_split_ip(struct in_addr ip) 
{
	return	printf("%d|!|%d|!|%d|!|%d", 
			(ip.s_addr & 0xFF), 
			((ip.s_addr >> 8) & 0xFF), 
			((ip.s_addr >> 16) & 0xFF), 
			((ip.s_addr >> 24) & 0xFF)); 
}

int	cmd_form_lan(qentry_t *req)
{
	return	0;
}
int cmd_get_lan(qentry_t *req)
{
	struct	net_info	info;

    qcgires_setcontenttype(req, "text/plain");
	get_net_info("eth0", &info);
	printf("disable");						printf("|!|");
	printf("%s", inet_ntoa(info.ip));		printf("|!|");
	printf("%s", inet_ntoa(info.netmask));	printf("|!|");
	printf("disable");						printf("|!|");
	printf("0.0.0.0");						printf("|!|");
	printf("0.0.0.0");						printf("|!|");
	printf("%s", inet_ntoa(info.gateway));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[0]));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[1]));	printf("|!|");
	printf("1");							printf("|!|");
	printf("192.168.100.1");				printf("|!|");
	printf("192.168.100.200");				printf("|!|");
	printf("3600");							printf("|!|");
	printf("1");							printf("|!|");

	return	0;
}

int cmd_get_config_dhcp(qentry_t *req)
{
	struct	net_info	info;

    qcgires_setcontenttype(req, "text/plain");
	printf("1");							printf("|!|");
	printf("192.168.100.1");				printf("|!|");
	printf("192.168.100.200");				printf("|!|");
	printf("3600");							printf("|!|");
	printf("1");							printf("|!|");

	return	0;
}

int cmd_set_config_dhcp(qentry_t *req)
{
	struct	net_info	info;

    qcgires_setcontenttype(req, "text/plain");
	printf("OK");							printf("|!|");
	printf("1");							printf("|!|");
	printf("192.168.100.1");				printf("|!|");
	printf("192.168.100.200");				printf("|!|");
	printf("3600");							printf("|!|");
	printf("1");							printf("|!|");

	return	0;
}
int	cmd_get_static_dhcp_data(qentry_t *req)
{
	struct	net_info	info;

    qcgires_setcontenttype(req, "text/plain");
	printf("2");							printf("|!|");
	printf("00:40:5c:01:01:01");			printf("|!|");
	printf("192.168.100.10");				printf("|!|");
	printf("00:40:5c:01:01:02");			printf("|!|");
	printf("192.168.100.11");				printf("|!|");

}

int cmd_set_lan(qentry_t *req)
{
	struct	net_info	info;

    qcgires_setcontenttype(req, "text/plain");
	printf("OK");							printf("|!|");
	get_net_info("eth0", &info);
	printf("enable");						printf("|!|");
	printf("%s", inet_ntoa(info.ip));		printf("|!|");
	printf("%s", inet_ntoa(info.netmask));	printf("|!|");
	printf("disable");						printf("|!|");
	printf("0.0.0.0");						printf("|!|");
	printf("0.0.0.0");						printf("|!|");
	printf("%s", inet_ntoa(info.gateway));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[0]));	printf("|!|");
	printf("%s", inet_ntoa(info.dns[1]));	printf("|!|");
	printf("1");							printf("|!|");
	printf("192.168.100.1");				printf("|!|");
	printf("192.168.100.200");				printf("|!|");
	printf("3600");							printf("|!|");
	printf("1");							printf("|!|");

	return	0;
}

int	cmd_get_dhcp_server_config(qentry_t *req)
{

	return	0;
}

int cmd_form_dhcp_server_config(qentry_t *req)
{
	return	0;
}

int	cmd_set_dhcp_server_config(qentry_t *req)
{

	return	0;
}

int cmd_get_ioctl(qentry_t *req)
{
    qcgires_setcontenttype(req, "text/plain");

	int stat = PMUC_Cmd(4, 0);

	printf("1");	printf("|!|");

	for(int i = 0 ; i < 4 ; i++)
	{
		if ((stat & (1 << i)) != 0)
		{
			printf("ON|!|");
		}
		else
		{
			printf("OFF|!|");
		}
	}

	return	0;
}


int	cmd_set_ioctl(qentry_t *req)
{
    char	*lpszID = (char *)req->getstr(req, "id", false);
    char	*lpszValue = (char *)req->getstr(req, "value", false);
	int		nID = -1;
	int		nValue = -1;
	
	qcgires_setcontenttype(req, "text/plain");

	nID = atoi(lpszID);
	if (strcmp(lpszValue, "ON") == 0)
	{
		nValue = 0;
	}
	else
	{
		nValue = 1;
	}

	if ((nID < 0 && 3 < nID) || ((nValue != 0) && (nValue != 1)))
	{
		printf("Invalid query\n");
		return	0;
	}

	
	int nState = PMUC_Cmd(nID, nValue);
	if (nState != -1)
		printf("%s|!|%s|!|%s", lpszID, (nState & (1 << nID))?"ON":"OFF", lpszValue); 
	else
		printf("%s|!|%s|!|%s", lpszID, "ERR", lpszValue); 

	return	0;
}

