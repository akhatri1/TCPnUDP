#include	"unp.h"
void executeCommand(char *);
int
main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	struct hostent *he;
	struct in_addr **addr_list;
	char hostname[50];
	FILE *fp;
	fp = fopen("/projects/dropbox/COEN236/P4/P4_AnkitKhatri/ipAddress1","w+");

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

	
	gethostname(&hostname,sizeof(hostname));
	printf("%s\n",hostname);
	he = gethostbyname(hostname);
	printf("Official name is: %s\n", he->h_name);
    	printf("    IP addresses: \n");
    	addr_list = (struct in_addr **)he->h_addr_list;
    	
        printf("%s \n", inet_ntoa(*addr_list[0]));
    	fprintf(fp,"%s\n",inet_ntoa(*addr_list[0]));
	fclose(fp);

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char	mesg[MAXLINE];

	for ( ; ; ) {
		len = clilen;
		n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		executeCommand(mesg);
		sendto(sockfd, mesg, 512, 0, pcliaddr, len);
		memset(mesg,'\0',MAXLINE);
	}
}

void executeCommand(char *com)
{
	char response[512], *filename, *opcode, ch,com1[512];
	int i=0, Status;
	FILE *fp1 = NULL;
	strcpy(com1,com);
	opcode = strtok(com1," ");
	filename = strtok(NULL," ");
	
	//printf("%s %d\n",filename,strlen(filename));
	if(strcmp(opcode,"Exit")==0)
	{
		
		kill(getpid(),SIGKILL);
		exit(0);
	}
	
	if((fp1 = fopen(filename,"r")) == NULL)
	{
		strcpy(com,"File not found :: Unsuccessful Operation\n");
		return;
	}
	fclose(fp1);
	
	fp1 = NULL;
	
	/*Read operation*/
	if(strcmp(opcode,"Read") == 0)
	{
		
		fp1 = fopen(filename,"r");
		while(1)
    	        {
      			ch = fgetc(fp1);

      			if(ch==EOF)
         		break;
      			else
          		{ response[i] = ch;
				i++;
			}
    		}
		response[i] = '\0';
		strcpy(com,response);
		fclose(fp1);
	}

	/*Delete operation*/	
	if(strcmp(opcode,"Delete") == 0)
	{
			
		
		/*Removing the requested file*/
		Status = remove(filename);
		if(Status == 0)
		{
			strcpy(com,"File is removed");
		}
		/*What if the file is present but the directory doesn't have write permission*/
		else
		{
			strcpy(com,"Error File cannot be deleted: Seems like the parent directory doesn't have write permissions");
		}
		
	}
	return; 
	/*Finishing the process*/
	
}
