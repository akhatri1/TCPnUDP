#include	"unp.h"
#define SERV_PORT 9877
int
main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	char *ServerIP;
	FILE *fp;
	fp = fopen("/projects/dropbox/COEN236/P4/P4_AnkitKhatri/ipAddress","r");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	fscanf(fp,"%s",ServerIP);
	fclose(fp);
	printf("%s\n",ServerIP);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, ServerIP, &servaddr.sin_addr);

	connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}

void
str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE],sendline1[MAXLINE], *checkOpcode;
	while(1){
		printf("Enter the command:\n");
		fgets(sendline, MAXLINE, fp);
		sendline[strlen(sendline)-1] = '\0';
		strcpy(sendline1,sendline);
		checkOpcode = strtok(sendline1," ");
		if(checkOpcode == NULL)
		{
			printf("Please enter a proper command\n");
		}
		else
		{
		/*Validating Opcode*/
			if(strcmp(checkOpcode,"Read") == 0 || strcmp(checkOpcode,"Delete") == 0 || strcmp(checkOpcode,"Exit") == 0)
			{
				if(strcmp(checkOpcode,"Exit") == 0)	
				{
					write(sockfd, sendline, strlen(sendline));
					printf("BYE\n");
					remove("/projects/dropbox/COEN236/P4/P4_AnkitKhatri/ipAddress");
					exit(0);
				}
				else
				{
					write(sockfd, sendline, strlen(sendline));
					//printf("Cleint :%s %d",sendline,strlen(sendline));
					if (read(sockfd, recvline, MAXLINE) == 0)
						perror("str_cli: server terminated prematurely");

					fputs(recvline, stdout);	
				}
			}
		}
	}
}
