#include "database-impl.h"

/** 
 * Function for refilling specified club with new article, quantity and price.
 * In case club doesn't exist, the club with specified club name is added as 
 * new club. In case article already exists in the club, the update query is 
 * processed else new article is inserted in the club. In case of error user
 * gets notified about the erro invocation. In case of success user gets notified
 * about success operation.
 */
char* fillNewArticle(Package *package, MYSQL *connection) {
	
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
	   if(updateArticle(connection, *foundArticle, clubId) == -1) {
		strcpy(serverResponse, "An error occured when refilling article.");
	   } else {
		strcpy(serverResponse, "Successfully refilled existing article in the specified club.");
	   }
	}

	return serverResponse;
}


/** 
 * Function for ordering drink for specified club. In case club doesn't exist
 * user gets notified for inserting wrong club name. In case artile not found
 * client gets notified about wrong drink name. If there is not enough quantity
 * appropriate message is returned. In case of success, success message is returned
 * to the client.
 */
char* orderDrink(Package *package, MYSQL *connection) {

	int clubId = checkIfClubExists(package->clubName);
	char* serverResponse = malloc(200);

	if(clubId == -1) {
	    disconnectFromMysql(connection);
	    return "Club doesn't exist. Try again.";
	}

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
	
	return serverResponse;
}

/** 
 * Function for checking one club's profit for the specified date.
 * If no orders are received on that date, client gets appropriate message,
 * else client gets the days profit for that club.
 */
char* checkDaysProfitForClub(Package *package, MYSQL *connection) {

	char query[MAX_QUERY_SIZE];
	char* serverResponse = malloc(200);

	bzero(query, MAX_QUERY_SIZE);

	strcpy(query, "SELECT SUM(article.price * orders.quantity) FROM club JOIN article ON club.id = article.clubId JOIN orders ON article.id = orders.articleId WHERE club.name = '");
	
	strcat(query, package->clubName);
	strcat(query, "' AND orders.orderDate='"); 
	strcat(query, package->date);
	strcat(query, "'");

	if (mysql_query(connection, query)) 
  	{
      		return get_error_message(connection);
  	}

	MYSQL_RES *result = mysql_store_result(connection);

	MYSQL_ROW row = mysql_fetch_row(result);
	mysql_free_result(result);

	if(!row[0]) {
	    return "There were no orders on specified date for specified club.";
	}

	strcpy(serverResponse, "Profit for the specified date was: ");
	strcat(serverResponse, row[0]);

	return serverResponse;
}

/** 
 * Function for checking one club's profit for the specified date.
 * If no orders are received on that date, client gets appropriate message,
 * else client gets the days profit for that club.
 */
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

/** 
 * Function for inserting new club.
 * Returns club id on success, -1 on fail.
 */
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

/** 
 * Function for inserting new article.
 * Returns article id on success, -1 on fail.
 */
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

	if (mysql_query(connection, query)) 
	{
	    return -1;
	}

	return mysql_insert_id(connection); 
}

/** 
 * Function for updating already existing article.
 * Returns 0 on success and -1 on fail.
 */
int updateArticle(MYSQL* connection, Article article, int clubId) {
	
	char query[MAX_QUERY_SIZE];
	char clubIdQueryParam[11], quantityQueryParam[11], articleIdQueryParam[11];
	
	strcpy(query, "UPDATE article SET `quantity` = ");
	sprintf(quantityQueryParam,"%d", article.quantity);
	strcat(query, quantityQueryParam);
	strcat(query, " WHERE id =  ");
	sprintf(articleIdQueryParam,"%d", article.id);
	strcat(query, articleIdQueryParam);

	if (mysql_query(connection, query)) 
	{
	    return -1;
	}
	
	return 0;
}

/** 
 * Function for deleting already existing article.
 * Returns 0 on success and -1 on fail.
 */
int deleteArticle(MYSQL* connection, Article article) {
	char query[MAX_QUERY_SIZE];
	char articleIdQueryParam[11];

	strcpy(query, "DELETE FROM article WHERE id = ");
	sprintf(articleIdQueryParam,"%d", article.id);
	strcat(query, articleIdQueryParam);	

	if (mysql_query(connection, query)) 
	{
	    return -1;
	}

	return 0;
}

/** 
 * Function for checking if article exists in the database.
 * Returns article data on success and NULL if article not found
 * or on failure.
 */
Article* checkIfArticleExistsInTheClub(MYSQL* connection, Article article, int clubId) {

	char query[MAX_QUERY_SIZE];
	char clubIdQueryParam[11], priceQueryParam[11], quantityQueryParam[11], articleIdQueryParam[11];
	Article* foundArticle = NULL;

	strcpy(query, "SELECT article.id, article.name, article.quantity, article.price FROM article INNER JOIN club ON article.clubId = club.id WHERE article.name='");
	strcat(query, article.name);
	strcat(query, "' and club.id=");
	sprintf(clubIdQueryParam,"%d", clubId);
	strcat(query, clubIdQueryParam);

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

/** 
 * Function for checking if club exists in the database.
 * Returns club id on success and -1 on failure.
 */
int checkIfClubExists(char *clubName) {
   
   char query[100];

   strcpy(query, "SELECT id FROM club WHERE name = '");
   strcat(query, clubName);
   strcat(query, "' LIMIT 1;");

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
