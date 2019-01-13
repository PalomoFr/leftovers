#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
	char chat[1024];
	gets(chat);
    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> <port> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       sleep(1);
       return main(argc, argv);
    }
    strcat(chat, "\0");
    
    int y;
    int po = send(sockfd, chat, 1024, NULL);
    char rec[1024];
    while(1){
		gets(chat);
		printf("envoie de : %s\n", chat);
		int u = send(sockfd, chat, 1024, NULL);
		if(u < 0){
			printf("EROROR\n");
		}
		if(y = recv(sockfd, rec, 1024, 0) > 0){
			printf("%s\n", rec);
		}
		memset(chat, '0',1024);
	}

    

    return 0;
}
