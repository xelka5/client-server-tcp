#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 

#include "common.h"

#define MAX_BUFFER_SIZE 1000

void orderDrink(Package** package, Response** response, int sockfd);
void refillDrink(Package** package, Response** response, int sockfd);
void getAccumulationForDay(Package** package, Response** response, int sockfd);
char* getClub(Package** package, Response** response, int sockfd);
void exitFromProgram(int sockfd);
void sendPackage(Package** package, Response** response, int sockfd);
void process(int sockfd);
