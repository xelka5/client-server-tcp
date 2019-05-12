#include <my_global.h>
#include <mysql.h>

#include "database-init.h"
#include "database-utils.h"

/* 
 * This file is used as initialization file for the database with two main operations:
 * 1. Create all database tables
 * 2. Seed tables with sample data
 *
 */
int main(int argc, char **argv)
{
  MYSQL* connection = connectToMysql();

  createDbTables(connection);
   
  seedDbTables(connection);
  
  disconnectFromMysql(connection);

  printf("Successfully initialized database!\n");
  return 0;
}

/* 
 * Function for creating all database tables
 * 
 */
void createDbTables(MYSQL* connection) {

  int i;
  char clubs[6][150] = { DROP_CLUBS_TABLE, CREATE_CLUBS_TABLE, DROP_ARTICLE_TABLE, CREATE_ARTICLE_TABLE, DROP_ORDERS_TABLE,
                          CREATE_ORDERS_TABLE };
  for(i=0;i<6;i++) {
    if (mysql_query(connection, clubs[i])) {
      finish_with_error(connection);
    }
  }

}

/* 
 * Function for seeding all database tables with example data
 * 
 */
void seedDbTables(MYSQL* connection) {
   if (mysql_query(connection, "INSERT INTO club VALUES(1,'Plazza')")) {
      finish_with_error(connection);
   }
   if (mysql_query(connection, "INSERT INTO club VALUES(2,'Club33')")) {
      finish_with_error(connection);
   }
   if (mysql_query(connection, "INSERT INTO article VALUES(1,'Becks', 5.0, 100, 2)")) {
      finish_with_error(connection);
   }
   if (mysql_query(connection, "INSERT INTO article VALUES(2,'Jack Daniels', 15.0, 15, 1)")) {
      finish_with_error(connection);
   }
   if (mysql_query(connection, "INSERT INTO article VALUES(3,'Vodka', 7.0, 32, 1)")) {
      finish_with_error(connection);
   }
   if (mysql_query(connection, "INSERT INTO orders VALUES(1, 1, 10, '2019-05-02')")) {
      finish_with_error(connection);
   }
   if (mysql_query(connection, "INSERT INTO orders VALUES(2, 1, 3, '2019-04-26')")) {
      finish_with_error(connection);
   }
   if (mysql_query(connection, "INSERT INTO orders VALUES(3, 1, 6, '2019-04-26')")) {
      finish_with_error(connection);
   }

}

