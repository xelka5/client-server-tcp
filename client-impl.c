#include "client-impl.h"

#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 

#define MAX_BUFFER_SIZE 1000

enum status{Ack, Nack};
enum operation{Order, Observe};

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
    Package* package = malloc(sizeof(Package));;
    Response* response = malloc(sizeof(Response));
    char buffer[MAX_BUFFER_SIZE];

    for (;;) { 
        
	bzero(buffer, sizeof(buffer));
  
        printf("Enter the article name: "); 
        scanf("%s", package->article.name);
	fflush(stdin);
	printf("Enter the article price: ");
	scanf("%lf", &package->article.price);     
	printf("Enter the article quantity: ");
	scanf("%d", &package->article.quantity);  

	memcpy(buffer, package, sizeof(Package));

	write(sockfd, buffer, sizeof(buffer)); 
        
	bzero(buffer, sizeof(buffer)); 
        read(sockfd, buffer, sizeof(buffer));
	memcpy(response, buffer, sizeof(buffer)); 
        printf("From server: %d\n", response->responseStatus);

        if ((strncmp(buffer, "exit", 4)) == 0) { 
           printf("Client Exit...\n"); 
           break; 
        } 
    } 
} 
