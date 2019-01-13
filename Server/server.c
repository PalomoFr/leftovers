#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <fcntl.h>

struct vector{
	int size;
	struct client* data;
	
};

struct client{
	int connfd;
	char* username;
	int renamed;
};

void init_client(struct client* cli){
	cli->connfd = 0;
	cli->username = calloc(50, sizeof(char));
	cli->renamed = 0;
}
void client_free(struct client* cli){
	free(cli->username);
}
void vector_init(struct vector* vect){
	vect->data = malloc(sizeof(struct client));
	vect->size = 0;
}
void vector_add(struct vector* vect, struct client e){
	vect->data[vect->size] = e;
	vect->size++;
	vect->data = realloc(vect->data, sizeof(struct client) * (vect->size + 1));
}
int vector_search(struct vector* vect, int e){
	for(int i = 0 ; i < vect->size ; i++){
		if(vect->data[i].connfd == e){
			return 1;
		}
	}
	return 0;
}
int vector_remove(struct vector* vect, int e){
	if(e < 0 || e >= vect->size){
		return 0;
	}
	for(int i = 0 ; i < vect->size ; i++){
		if(i >= e && i < vect->size - 1){
			vect->data[i] = vect->data[i+1];
		}
	}
	vect->size--;
	return 1;
}
void vector_print(struct vector* vect){
	printf("taille = %d\n", vect->size);
	for(int i = 0 ; i < vect->size ; i++){
		printf("Client %d {Socket : %d ---> username : %s}\n",i, vect->data[i].connfd, vect->data[i].username);
	}
}


int main(int argc, char *argv[])
{
	
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    //int j = sprintf(sendBuff, "Test Message Server \n");ù
	printf("Server Launched Port : %s \n", argv[1]);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(listenfd, F_SETFL, SOCK_NONBLOCK);
    memset(&serv_addr, '0', sizeof(serv_addr));
     

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    int nb = 0;
    char buf[1024];
    memset(buf, '§', 1024);
	struct vector vect;
	int u = 0;
	vector_init(&vect);/*
	struct client cz = {4, "USER", 0};
	struct client cA = {5, "USER2", 0};
	struct client cB = {6, "USER3", 0};
	struct client cC = {7, "USER4", 0};
	vector_add(&vect, cz);
	vector_add(&vect, cA);
	vector_add(&vect, cB);
	vector_add(&vect, cC);
	vector_print(&vect);
	vector_remove(&vect, 3);
	vector_print(&vect);*/
    while(1)
    {
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		fcntl(connfd, F_SETFL, SOCK_NONBLOCK);
		if(!vector_search(&vect, connfd) && connfd != -1){
			struct client cli;
			init_client(&cli);
			cli.renamed = 0;
			cli.connfd = connfd;
			printf("Number %d connecting... \n", connfd);
			vector_add(&vect, cli);
			printf("List of connected : \n");
			//fcntl(vect.data[vect.size - 1].connfd, F_SETFL, SOCK_NONBLOCK);
			for(int i = 0; i < vect.size ; i++){
				if(vect.data[i].connfd == connfd){
					printf("---> YOU : connected\n");
				}else{
					printf("---> %s : connected\n", vect.data[i].username);
				}
			}
		}
		
		
		
		for(int i = 0; i < vect.size ; i++){
			int y = recv(vect.data[i].connfd, buf, 1024, 0);
			
			if(y > 0){
				if(vect.data[i].renamed == 0){
					for(int j = 0 ; j < 1024 ; j++){
						vect.data[i].username[j] = buf[j];
						if(buf[j] == '\0'){
							break;
						}
					}
					vect.data[i].renamed = 1;
					//printf("Username changed in %s \n", vect.data[i].username);
					memset(buf, '0', 1024);
				}
				else{
					
					char* lop = malloc(sizeof(char) * 1075);
					snprintf(lop,1050, "[%s]: %s\n", vect.data[i].username, buf);
					printf("%s\n", lop);
					for(int k = 0; k < vect.size ; k++){
						if(vect.data[k].connfd != vect.data[i].connfd){
							int u = send(vect.data[k].connfd, lop, 1075, NULL);
							if(u < 0){
								printf("Error\n");
							}
						}
					}
					free(lop);
					memset(buf, '0', 1024);
				}
				
			}
			if(y == 0){
				printf("deconnexion from %s\n", vect.data[i].username);
				
				shutdown(vect.data[i].connfd, 2);
				//client_free(&vect.data[i]);
				vector_remove(&vect, i);
			}
			memset(buf, '0', 1024);
		}
		
		

		
        
    }
}
