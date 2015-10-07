I've tried to compile and run the following simple program:

#include <stdio.h>
int main() { printf("Ok\n"); return(0); }

I used the command:

	armv5-linux-gcc -o try -O2 -DOVR_DBL_DIG=14 -I/usr/local/include try.c
	/home/xtra/work/ftm-50s2/perl/perl-5.22.0/Cross/run-ssh ./try

and I got the following output:

ssh: connect to host 192.168.1.10 port 22: Connection refused
ssh: connect to host 192.168.1.10 port 22: Connection refused
The program compiled OK, but exited with status 255.
