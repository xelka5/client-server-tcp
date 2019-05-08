#include "server-impl.h"
#include "mysql.h"
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <stdio.h>

#define MAX_BUFFER_SIZE 1000 

enum operation{Order, Refill, GetAccumulation, GetClub};
enum status{Ack, Nack};

typedef struct Article {
   char name[50];
   double price;
   int quantity;
}Article;  

typedef struct Package {
   enum operation packageType;
   char clubName[50];
   struct Article article;
   char date[10];
}Package;

typedef struct Response {
   enum status responseStatus;
   char responsePayload[100];
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

	printf("Package type: %d\n", package->packageType);
	printf("Received club name: %s\n", package->clubName);
	printf("Received date: %s\n", package->date);

	switch(package->packageType) {
	  case 0:
		response->responseStatus = Ack;
		strcpy(response->responsePayload, "Order successful");
		printf("From client: %s %lf %d\n", package->article.name, package->article.price, package->article.quantity);
		break;
	  case 1:
		response->responseStatus = Ack;
		strcpy(response->responsePayload, "Refill successful");
		printf("From client: %s %lf %d\n", package->article.name, package->article.price, package->article.quantity);
		break;
	  case 2:
		response->responseStatus = Ack;
		strcpy(response->responsePayload, "Get accumulation successful");
		printf("From client: %s %lf %d\n", package->article.name, package->article.price, package->article.quantity);
		break;	
        }
        
        bzero(buffer, MAX_BUFFER_SIZE); 
        
        memcpy(buffer, response, sizeof(Response));
  
        write(sockfd, buffer, sizeof(buffer)); 
  
    } 
}
