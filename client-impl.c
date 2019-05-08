#include "client-impl.h"

#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 

#define MAX_BUFFER_SIZE 1000

enum status{Ack, Nack};
enum operation{Order, Refill, GetAccumulation};

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

void orderDrink(Package** package, Response** response, int sockfd);
void refillDrink(Package** package, Response** response, int sockfd);
void getAccumulationForDay(Package** package, Response** response, int sockfd);
char* getClub(Package** package, Response** response, int sockfd);
void exitFromProgram(int sockfd);

void sendPackage(Package** package, Response** response, int sockfd);

void process(int sockfd) 
{ 
    Package* package = NULL;
    Response* response = NULL;
    
    int operation;

   // do {
	//
	
	//if(clubName == NULL) {
	 //  printf("Club name doesn't exist in our database. Try again!\n");
      //  }
   // }while(clubName == NULL);

    while(1) { 
	   //clubName = getClub(&package, &response, sockfd);
	   //clubName="name";
	   //refillDrink(&package, &response, sockfd, clubName);

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

void getAccumulationForDay(Package** package, Response** response, int sockfd) {
	
	*package = malloc(sizeof(Package));
	*response = malloc(sizeof(Response));	

	(*package)->packageType = GetAccumulation;
	
	printf("Enter club name: ");
	scanf(" %s", (*package)->clubName);

	printf("Enter the date: ");
	scanf(" %s", (*package)->

	sendPackage(package, response, sockfd);

        printf("From server status: %d\n", (*response)->responseStatus);
	printf("From server response: %s\n", (*response)->responsePayload);

	free(*package);
	free(*response);
}

void exitFromProgram(int sockfd) {
	close(sockfd);
	printf("Success exit!\n");
	exit(0);
}

char* getClub(Package** package, Response** response, int sockfd) {

	char* clubName = malloc(150);

	*package = (Package*) malloc(sizeof(Package));
	*response = (Response*) malloc(sizeof(Response));

	//(*package)->packageType = GetClub;	
	
	printf("Enter club name: \n");
	scanf("%s", (*package)->clubName);

	sendPackage(package, response, sockfd);

	printf("Response: %s\n", (*response)->responsePayload);

	strcpy(clubName, (*response)->responsePayload);
	printf("Returned name: %s\n", clubName);
	
	free(*package);
	free(*response);
	
	return clubName;

}

void sendPackage(Package** package, Response** response, int sockfd) {

	char buffer[MAX_BUFFER_SIZE];
	bzero(buffer, sizeof(buffer));

	memcpy(buffer, (*package), sizeof(buffer));

	write(sockfd, buffer, sizeof(buffer)); 
        
	bzero(buffer, sizeof(buffer)); 
        read(sockfd, buffer, sizeof(buffer));
	memcpy((*response), buffer, sizeof(buffer));
}





 
