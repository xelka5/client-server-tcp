#include "server-impl.h"
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <stdio.h>

#define MAX_BUFFER_SIZE 1000 

enum operation{Order, Observe};
enum status{Ack, Nack};

typedef struct Article {
   char name[50];
   double price;
   int quantity;
}Article;  

typedef struct Package {
   enum operation packageType;
   struct Article article;
   long timestamp;
}Package;

typedef struct Response {
   enum status responseStatus;
}Response;

void process(int sockfd) 
{ 
    
    Package* package = malloc(sizeof(Package));
    Response* response = malloc(sizeof(Response));
    char buffer[MAX_BUFFER_SIZE];

    for (;;) { 
        
	bzero(buffer, MAX_BUFFER_SIZE); 
	
        read(sockfd, buffer, sizeof(buffer)); 
	printf("READING\n");
        memcpy(package, buffer, sizeof(buffer));

	if(package->article.price == 0) {
	    response->responseStatus = Nack;
        } else {
	    response->responseStatus = Ack;
	    printf("From client: %s %lf %d\n", package->article.name, package->article.price, package->article.quantity);
        }
        
        bzero(buffer, MAX_BUFFER_SIZE); 
        
        memcpy(buffer, response, sizeof(Response));
  
        write(sockfd, buffer, sizeof(buffer)); 
  
        if (strncmp("exit", buffer, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
}
