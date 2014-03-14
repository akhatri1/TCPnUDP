#include	"unp.h"

int
main(int argc, char **argv)
{
	int sockfd, i=0;
	struct sockaddr_in	servaddr;
	char *ServerIP, arr[20], ch, *com;
	/*if (argc != 2)
		perror("usage: udpcli <IPaddress>");*/
	FILE *fp1;
	fp1 = fopen("/projects/dropbox/COEN236/P4/P4_AnkitKhatri/ipAddress1","r+");
	while( ( ch = fgetc(fp1) ) != EOF ){
      	
	arr[i] = ch;
	i++;
	}
	arr[i] = '\0';
	fclose(fp1);
	printf("%s \n",arr);
	//strncpy(com,arr,strlen(arr));
	//com[strlen(arr)-1] = '\0';
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,argv[1], &servaddr.sin_addr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

	exit(0);
}



void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1], sendline1[MAXLINE], *checkOpcode;
	while(1){
		printf("Enter the command:\n");
		fgets(sendline, MAXLINE, fp);
	//while (fgets(sendline, MAXLINE, fp) != NULL) {
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
					sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
					printf("BYE\n");
					remove("/projects/dropbox/COEN236/P4/P4_AnkitKhatri/ipAddress1");
					exit(0);
				}
				else
				{
					sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

					n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);

					recvline[n] = 0;	/* null terminate */
					fputs(recvline, stdout);
				}
			}
                       else
			{
				printf("Please specify the Opcode Correctly\n");
				printf("\tRead\n");
				printf("\tDelete\n");
				printf("\tExit\n");
			}
		}
	}
}
