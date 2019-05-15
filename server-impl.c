#include "server-impl.h"

/** 
 * Function for processing tcp socket connection between the server
 * and the subscribed client.
 */
void process(int sockfd) 
{ 
    
    Package* package = malloc(sizeof(Package));
    MYSQL* connection = connectToMysql();
    memset(package, 0, sizeof(Package));
    Response* response = malloc(sizeof(Response));
    char buffer[MAX_BUFFER_SIZE];

    for (;;) { 
        
	bzero(buffer, MAX_BUFFER_SIZE); 
	
        read(sockfd, buffer, sizeof(buffer)); 
        memcpy(package, buffer, sizeof(buffer));

	switch(package->packageType) {
	  case 0:
		response->responseStatus = Ack;
		strcpy(response->responsePayload, orderDrink(package, connection));
		printf("From client: %s %lf %d\n", package->article.name, package->article.price, package->article.quantity);
		break;
	  case 1:
		response->responseStatus = Ack;
		printf("From client: %s %lf %d\n", package->article.name, package->article.price, package->article.quantity);
		strcpy(response->responsePayload, fillNewArticle(package, connection));
		break;
	  case 2:
		response->responseStatus = Ack;
		printf("From client: %s %s\n", package->clubName, package->date);
		strcpy(response->responsePayload, checkDaysProfitForClub(package, connection));
		break;	
        }
        
        bzero(buffer, MAX_BUFFER_SIZE); 
        
        memcpy(buffer, response, sizeof(Response));
  
        write(sockfd, buffer, sizeof(buffer)); 
  
    } 

    disconnectFromMysql(connection);
}

