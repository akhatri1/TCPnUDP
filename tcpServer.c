#include	"unp.h"


void executeCommand(char *);

#define NIPQUAD(addr) \
         ((unsigned char *)&addr)[0], \
         ((unsigned char *)&addr)[1], \
         ((unsigned char *)&addr)[2], \
         ((unsigned char *)&addr)[3]


int
main(int argc, char **argv)
{
	int listenfd, connfd,i;
	pid_t	childpid;
	socklen_t clilen, sa_len;
	struct sockaddr_in cliaddr, servaddr, sa;
	struct hostent *he;
	struct in_addr **addr_list;
	char hostname[50];
	FILE *fp;
	fp = fopen("/projects/dropbox/COEN236/P4/P4_AnkitKhatri/ipAddress","w+");
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);
	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	 
	gethostname(&hostname,sizeof(hostname));
	printf("%s\n",hostname);
	he = gethostbyname(hostname);
	printf("Official name is: %s\n", he->h_name);
    	printf("    IP addresses: ");
    	addr_list = (struct in_addr **)he->h_addr_list;
    	
        printf("%s \n", inet_ntoa(*addr_list[0]));
    	fprintf(fp,"%s\n",inet_ntoa(*addr_list[0]));
	fclose(fp);
	/*if (getsockname(listenfd, (SA *)&sa, &sa_len) == -1) {
      		perror("getsockname() failed");
      		return -1;
   	}
	printf("Local IP address is: %d\n", sa.sin_port);*/
	

	listen(listenfd, LISTENQ);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *) &cliaddr, &clilen);

		if ( (childpid = fork()) == 0) {	/* child process */
			close(listenfd);	/* close listening socket */
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		close(connfd);			/* parent closes connected socket */
	}
}

void
str_echo(int sockfd)
{
	ssize_t	n;
	char	buf[MAXLINE];
	char buf1[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0){
		//printf("%s %d",buf,strlen(buf));
		//strncpy(buf1,buf,n);
		executeCommand(buf);
		//printf("Server2 :%s %d",buf, strlen(buf));
		write(sockfd, buf, 512);
		buf[9] = '\0';
		memset(buf,'\0',MAXLINE);
	}
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		perror("str_echo: read error");
}

/*Function for Reading the file, Deleting the file*/
void executeCommand(char *com)
{
	char response[512], *filename, *opcode, ch,com1[512];
	int i=0, Status;
	FILE *fp1 = NULL;
	strcpy(com1,com);
	opcode = strtok(com1," ");
	filename = strtok(NULL," ");
	
	if(strcmp(opcode,"Exit")==0)
	{
		
		kill(getppid(),SIGKILL);
		exit(0);
	}
	
	//printf("%s %d\n",filename,strlen(filename));
	
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
