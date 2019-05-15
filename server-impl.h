#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h> 
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include<signal.h>
#include <my_global.h>
#include <mysql.h>
#include "database-utils.h"
#include "database-impl.h"

#define MAX_BUFFER_SIZE 1000 

void process(int sockfd);
