#include <my_global.h>
#include <mysql.h>
#include <string.h>

#include "database-utils.h"
#include "database-impl.h"

/*int main() {

     Package* package = malloc(sizeof(Package));
     strcpy(package->article.name, "Becks");
     package->article.quantity = 87;
     package->article.price = 5;
     strcpy(package->clubName,"Club331");
     strcpy(package->date,"2019-05-12");
     //printf("%s\n", fillNewArticle(package));
     printf("%s\n", checkDaysProfitForClub(package));
     //printf("%s\n", orderDrink(package));
}*/

char* fillNewArticle(Package* package) {
	
	MYSQL* connection = connectToMysql();
	int clubId;
	char* serverResponse = malloc(200);
   
	clubId = checkIfClubExists(package->clubName);

	if(clubId == -1) {
	    clubId = insertNewClub(connection, package->clubName);
	}

	Article* foundArticle = checkIfArticleExistsInTheClub(connection, package->article, clubId);
 	
	if(foundArticle == NULL) {	
	   if(insertNewArticle(connection, package->article, clubId) == -1) {
		strcpy(serverResponse, "An error occured when refilling article.");
	    } else {
		strcpy(serverResponse, "Successfully added new article to the specified club");
	    }
	} else {
           foundArticle->quantity = foundArticle->quantity + package->article.quantity;
	   printf("Existing quantity: %d, adding: %d\n", foundArticle->quantity, package->article.quantity);
	   if(updateArticle(connection, *foundArticle, clubId) == -1) {
		strcpy(serverResponse, "An error occured when refilling article.");
	   } else {
		strcpy(serverResponse, "Successfully refilled existing article in the specified club.");
	   }
	}
	
  	disconnectFromMysql(connection);
	return "test";
}

char* orderDrink(Package* package) {

	MYSQL* connection = connectToMysql();
	int clubId = checkIfClubExists(package->clubName);
	char* serverResponse = malloc(200);

	if(clubId == -1) {
	    disconnectFromMysql(connection);
	    return "Club doesn't exist. Try again.";
	}

	printf("ClubID: %d\n", clubId);

	Article* foundArticle = checkIfArticleExistsInTheClub(connection, package->article, clubId);

	if(foundArticle == NULL) {
	    strcpy(serverResponse, "Drink not found for specified club.");
	} else if(package->article.quantity > foundArticle->quantity) {
	    strcpy(serverResponse, "There is not enough quantity of the drink in the club.");
	} else if(package->article.quantity == foundArticle->quantity) {
	     if(order(connection, *foundArticle, package->article.quantity , DELETE_ARTICLE_FLAG, clubId) == -1) {
		strcpy(serverResponse, "An error occured ordering drink. Try again.");
	     } else {
		strcpy(serverResponse, "Drink ordered successfully.");
 	     }
	} else {
	     if(order(connection, *foundArticle, package->article.quantity, UPDATE_ARTICLE_FLAG, clubId) == -1) {
		strcpy(serverResponse, "An error occured ordering drink. Try again.");
	     } else {
		strcpy(serverResponse, "Drink ordered successfully.");
	     }
	}
	
	disconnectFromMysql(connection);
	return serverResponse;
}

char* checkDaysProfitForClub(Package *package) {

	char query[MAX_QUERY_SIZE];
  	MYSQL* connection = connectToMysql();
	char* serverResponse = malloc(200);


	bzero(query, MAX_QUERY_SIZE);

	strcpy(query, "SELECT SUM(article.price * orders.quantity) FROM club JOIN article ON club.id = article.clubId JOIN orders ON article.id = orders.articleId WHERE club.name = '");
	
	strcat(query, package->clubName);
	strcat(query, "' AND orders.orderDate='"); 
	strcat(query, package->date);
	strcat(query, "'");

	printf("Query: %s\n", query);

	if (mysql_query(connection, query)) 
  	{
      		return get_error_message(connection);
  	}

	MYSQL_RES *result = mysql_store_result(connection);

	MYSQL_ROW row = mysql_fetch_row(result);
	mysql_free_result(result);
	disconnectFromMysql(connection);

	if(!row[0]) {
	    return "There were no orders on specified date for specified club.";
	}

	strcpy(serverResponse, "Profit for the specified date was: ");
	strcat(serverResponse, row[0]);

	return serverResponse;


}

int order(MYSQL* connection, Article article, int quantityOrdered, int deleteArticleFlag, int clubId) {
	
	char query[MAX_QUERY_SIZE];
	char articleIdQueryParam[11], quantityQueryParam[11];

        strcpy(query, "INSERT INTO orders(`articleId`, `quantity`, `orderDate`) VALUES(");
	sprintf(articleIdQueryParam,"%d", article.id);
	strcat(query, articleIdQueryParam);
	strcat(query, ", ");
	sprintf(quantityQueryParam,"%d", quantityOrdered);
	strcat(query, quantityQueryParam);
	strcat(query, ",CURDATE())");

	printf("Order query: %s\n", query);

	if (mysql_query(connection, query)) 
	{
	    return -1;
	}

	if(deleteArticleFlag == 1) {
	    if(deleteArticle(connection, article) == -1) {
		return -1;
	    }    
	} else {
	    article.quantity = article.quantity - quantityOrdered;
	    if(updateArticle(connection, article, clubId) == -1) {
		return -1;
	    }
	}
	return 0;

}

int insertNewClub(MYSQL* connection, char *clubName) {

	char query[MAX_QUERY_SIZE];
        strcpy(query, "INSERT INTO club(`name`) VALUES('");
	strcat(query, clubName);
   	strcat(query, "');");

	if (mysql_query(connection, query)) 
  	{
      	    return -1;
  	}
        return mysql_insert_id(connection);
}

int insertNewArticle(MYSQL* connection, Article article, int clubId) {

	char query[MAX_QUERY_SIZE];
	char clubIdQueryParam[11], quantityQueryParam[11], priceQueryParam[11];
	
	strcpy(query, "INSERT INTO article(`name`,`quantity`,`price`,`clubId`) VALUES('");
	strcat(query, article.name);
	strcat(query, "', ");
	sprintf(priceQueryParam,"%f", article.price);
	strcat(query, priceQueryParam);
	strcat(query, ", ");
	sprintf(quantityQueryParam,"%d", article.quantity);
	strcat(query, quantityQueryParam);
	strcat(query, ", ");
	sprintf(clubIdQueryParam,"%d", clubId);
	strcat(query, clubIdQueryParam);
	strcat(query, ")");

	printf("Query: %s\n", query);

	if (mysql_query(connection, query)) 
	{
	    return -1;
	}

	return mysql_insert_id(connection); 
}

int updateArticle(MYSQL* connection, Article article, int clubId) {
	
	char query[MAX_QUERY_SIZE];
	char clubIdQueryParam[11], quantityQueryParam[11], articleIdQueryParam[11];
	
	strcpy(query, "UPDATE article SET `quantity` = ");
	sprintf(quantityQueryParam,"%d", article.quantity);
	strcat(query, quantityQueryParam);
	strcat(query, " WHERE id =  ");
	sprintf(articleIdQueryParam,"%d", article.id);
	strcat(query, articleIdQueryParam);

	printf("Query: %s\n", query);

	if (mysql_query(connection, query)) 
	{
	    return -1;
	}
}

int deleteArticle(MYSQL* connection, Article article) {
	char query[MAX_QUERY_SIZE];
	char articleIdQueryParam[11];

	strcpy(query, "DELETE FROM article WHERE id = ");
	sprintf(articleIdQueryParam,"%d", article.id);
	strcat(query, articleIdQueryParam);	
	printf("Delete query: %s\n", query);

	if (mysql_query(connection, query)) 
	{
	    return -1;
	}

	return 0;
}

Article* checkIfArticleExistsInTheClub(MYSQL* connection, Article article, int clubId) {

	char query[MAX_QUERY_SIZE];
	char clubIdQueryParam[11], priceQueryParam[11], quantityQueryParam[11], articleIdQueryParam[11];
	Article* foundArticle = NULL;

	strcpy(query, "SELECT article.id, article.name, article.quantity, article.price FROM article INNER JOIN club ON article.clubId = club.id WHERE article.name='");
	strcat(query, article.name);
	strcat(query, "' and club.id=");
	sprintf(clubIdQueryParam,"%d", clubId);
	strcat(query, clubIdQueryParam);

	printf("Query: %s\n", query);

	if (mysql_query(connection, query)) 
	{
	    return foundArticle;
	}
	
	MYSQL_RES *result = mysql_store_result(connection);

	if (result == NULL) 
	{
	    return foundArticle;
	}

	int num_fields = mysql_num_fields(result);
	

	if(mysql_num_rows(result) == 0) {
	    return foundArticle;
   	} else {
	    MYSQL_ROW row = mysql_fetch_row(result);
	    foundArticle = malloc(sizeof(Article));
	    foundArticle->id = atoi(row[0]);
	    strcpy(foundArticle->name, row[1]);
	    foundArticle->quantity = atoi(row[2]); 
	    foundArticle-> price = atoi(row[3]);
	}
	
	return foundArticle;
}

int checkIfClubExists(char *clubName) {
   
   char query[100];

   strcpy(query, "SELECT id FROM club WHERE name = '");
   strcat(query, clubName);
   strcat(query, "' LIMIT 1;");
   printf("QUERY: %s\n", query);

   MYSQL* connection = connectToMysql();

   if (mysql_query(connection, query)) 
   {
      return -1;
   }
  
   MYSQL_RES *result = mysql_store_result(connection);
  
   if (result == NULL) 
   {
      return -1;
   }

   int num_fields = mysql_num_fields(result);

   if(mysql_num_rows(result) == 0) {
      return -1;
   }

   MYSQL_ROW row;
  
   row = mysql_fetch_row(result);

   mysql_free_result(result);
   disconnectFromMysql(connection);

   return atoi(row[0]);
   
}
