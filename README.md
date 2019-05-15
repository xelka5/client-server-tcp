# TCP Client-Server System Programming

Client-Server communication over socket implementation listening on specified host and port.

### Prerequisites

1. MYSQL database server
2. GCC compailer

### Installing

First install MYSQL server and gcc compailer.

Server side build command:
	gcc -o server server.c server-impl.c database-impl.c database-utils.c `mysql_config --cflags --libs`

Client side build command:
	gcc -o client client.c client-impl.c

### Running

Client side: ./client

Server side: ./server

## Author

* **Dusan Cvetkov** - *Initial work* - https://github.com/xelka5


