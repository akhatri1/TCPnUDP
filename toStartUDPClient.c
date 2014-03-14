#include	"unp.h"

int
main(int argc, char **argv)
{
	char *ServerIP;
	FILE *fp;
	fp = fopen("/projects/dropbox/COEN236/P4/P4_AnkitKhatri/ipAddress1","r");
	fscanf(fp,"%s",ServerIP);
	fclose(fp);
	execl("./UDPClient1","./UDPClient1", ServerIP);
	printf("%s\n",ServerIP);
	exit(0);
}
