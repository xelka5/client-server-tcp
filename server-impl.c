#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <stdio.h>
#include <my_global.h>
#include <mysql.h>

#include "database-utils.h"
#include "database-impl.h"
#include "server-impl.h"

#define MAX_BUFFER_SIZE 1000 

void process(int sockfd) 
{ 
    
    Package* package = malloc(sizeof(Package));
    Response* response = malloc(sizeof(Response));
    char buffer[MAX_BUFFER_SIZE];

    for (;;) { 
        
	bzero(buffer, MAX_BUFFER_SIZE); 
	
        read(sockfd, buffer, sizeof(buffer)); 
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
		printf("From client: %s %s\n", package->clubName, package->date);
		printf("Response db: %s\n", checkDaysProfitForClub(package, package->date));
		printf("AFTER!!\n");
		//strcpy(response->responsePayload, checkDaysProfitForClub(package));
		break;	
        }
        
        bzero(buffer, MAX_BUFFER_SIZE); 
        
        memcpy(buffer, response, sizeof(Response));
  
        write(sockfd, buffer, sizeof(buffer)); 
  
    } 
}
