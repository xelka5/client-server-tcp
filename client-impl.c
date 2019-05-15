#include "client-impl.h"

/** 
 * Function for processing tcp socket connection between the server
 * and the subscribed client.
 */
void process(int sockfd) 
{ 
    Package* package = NULL;
    Response* response = NULL;
    
    int operation;


    while(1) { 
	   printf("Choose operation: \n1.Order\n2.Refill\n3.Get days accumulation\n4.Exit\n");
	   scanf("%d",&operation);
	   switch(operation) {
	    case 1:
	      orderDrink(&package, &response,sockfd);
	      break;
            case 2:
	      refillDrink(&package, &response, sockfd);
	      break;
	    case 3:
	      getAccumulationForDay(&package, &response, sockfd);
	      break;
	    case 4:
	      exitFromProgram(sockfd);
	      break;
            default:
              printf("Wrong option!\n");
              break;
 	   }
    } 
}

/** 
 * Function for managing client side data input when ordering drink.
 * Data:
 *   1. Club name
 *   2. Article name
 *   3. Article quantity
 */
void orderDrink(Package** package, Response** response, int sockfd) {

	*package = malloc(sizeof(Package));
	*response = malloc(sizeof(Response));	

	(*package)->packageType = Order;
	
	printf("Enter club name: ");
	scanf(" %s", (*package)->clubName);
	printf("Enter the article name: "); 
        scanf(" %s", (*package)->article.name);
	fflush(stdin);
	printf("Enter the article quantity: ");
	scanf("%d", &(*package)->article.quantity);  

	sendPackage(package, response, sockfd);

        printf("From server status: %d\n", (*response)->responseStatus);
	printf("From server response: %s\n", (*response)->responsePayload);

	free(*package);
	free(*response);	
}

/** 
 * Function for managing client side data input when refilling drink.
 * Data:
 *   1. Club name
 *   2. Article name
 *   3. Article price
 *   4. Article quantity
 */
void refillDrink(Package** package, Response** response, int sockfd) {
	
	*package = malloc(sizeof(Package));
	*response = malloc(sizeof(Response));	

	(*package)->packageType = Refill;

	printf("Enter club name: ");
	scanf(" %s", (*package)->clubName);
	printf("Enter the article name: "); 
        scanf(" %s", (*package)->article.name);
	printf("Enter the article price: ");
	scanf("%lf", &(*package)->article.price);     
	fflush(stdin);   
	printf("Enter the article quantity: ");
	scanf("%d", &(*package)->article.quantity);  

	sendPackage(package, response, sockfd);

        printf("From server status: %d\n", (*response)->responseStatus);
	printf("From server response: %s\n", (*response)->responsePayload);

	free(*package);
	free(*response);	
}

/** 
 * Function for managing client side data input when checking days profit.
 * Data:
 *   1. Club name
 *   2. Date
 */
void getAccumulationForDay(Package** package, Response** response, int sockfd) {
	
	*package = malloc(sizeof(Package));
	*response = malloc(sizeof(Response));	

	(*package)->packageType = GetAccumulation;
	
	printf("Enter club name: ");
	scanf(" %s", (*package)->clubName);

	printf("Enter the date: ");
	scanf(" %s", (*package)->date);

	sendPackage(package, response, sockfd);

        printf("From server status: %d\n", (*response)->responseStatus);
	printf("From server response: %s\n", (*response)->responsePayload);

	free(*package);
	free(*response);
}

/** 
 * Function for closing socket connection
 */
void exitFromProgram(int sockfd) {
	close(sockfd);
	printf("Success exit!\n");
	exit(0);
}

/** 
 * Utility function for sending package and receiving response 
 * from the server.
 */
void sendPackage(Package** package, Response** response, int sockfd) {

	char buffer[MAX_BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));

	memcpy(buffer, (*package), sizeof(buffer));

	write(sockfd, buffer, sizeof(buffer)); 
        
	bzero(buffer, sizeof(buffer)); 
        read(sockfd, buffer, sizeof(buffer));
	memcpy((*response), buffer, sizeof(buffer));
}





 
