#include <my_global.h>
#include <mysql.h>

#include "database-utils.h"

/* 
 * Function for opening database connection
 * 
 */
MYSQL* connectToMysql() {

  MYSQL *connection = mysql_init(NULL);
  if (connection == NULL) {
      fprintf(stderr, "%s\n", mysql_error(connection));
      exit(1);
  }

  if (mysql_real_connect(connection, HOST, USERNAME, PASSWORD, 
          DB_NAME, 0, NULL, 0) == NULL) {
      fprintf(stderr, "%s\n", mysql_error(connection));
      mysql_close(connection);
      exit(1);
  }
  return connection;
}

/* 
 * Function for closing the existing database connection
 * 
 */
void disconnectFromMysql(MYSQL* connection) {
   mysql_close(connection);
}

/* 
 * Function for handling erros occured in the query processing
 * 
 */  
void finish_with_error(MYSQL *connection)
{
  fprintf(stderr, "%s\n", mysql_error(connection));
  mysql_close(connection);
  exit(1);        
}

/* 
 * Function for returning error message to the client
 * 
 */ 
char* get_error_message(MYSQL *connection) {
  char* message = malloc(100);
  strcpy(message, mysql_error(connection));
  mysql_close(connection);
  return message;
}

