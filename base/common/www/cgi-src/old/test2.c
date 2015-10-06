#include <stdio.h>
#include <unistd.h>
#if 0
void main(void)
{
	FILE	*fp;
	char	cmd[256];
	char	line[256], *ptr;

	sprintf(cmd, "../cgi-bin/lan_conf.sh %s", "eth1");
	fp = popen(cmd, "r");

	while(fgets(line, sizeof(line), fp) != NULL)
	{
		printf("line = %s", line);
	}

	pclose(fp);
}
#else
void main(void)
{
	while(1)
	{
		fprintf(stdout, ".");
		fflush(stdout);
		usleep(1000);
	}
}
#endif
