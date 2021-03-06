#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include "server-impl.h"

#define PORT 9898
#define HOST "127.0.0.1" 
#define SA struct sockaddr 

/** 
 * Server side main processing service implemented
 * by socket connection over specified host and port.
 * All clients are handled by the server at their own
 * processes. The server is accepting multiple clients
 * at once. When all clients disconnect from the server,
 * socket is closed.
 */
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
    pid_t childpid;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Socket creation failed...\n"); 
        exit(0); 
    } else {
        printf("Socket successfully created..\n"); 
    }

    bzero(&servaddr, sizeof(servaddr)); 
  

    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr(HOST); 
    servaddr.sin_port = htons(PORT); 
  
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Socket bind failed...\n"); 
        exit(0); 
    } else {
        printf("Socket successfully binded..\n"); 
    }
  
    if ((listen(sockfd, 10)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } else {
        printf("Server listening..\n"); 
    }

    while(1) {
 
        connfd = accept(sockfd, (SA*)&cli, &addr_size); 
	if (connfd < 0) { 
	   printf("Server acccept failed...\n"); 
	   exit(0); 
	} else {
	   printf("Connection accepted from %s:%d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
	}

	if((childpid = fork()) == 0) { 
	  process(connfd); 
	}

    }
  
    close(sockfd); 
} 
