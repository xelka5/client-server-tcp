#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include "common.h"
#include "database-utils.h"

#define MAX_QUERY_SIZE 1000
#define UPDATE_ARTICLE_FLAG 0
#define DELETE_ARTICLE_FLAG 1

char* fillNewArticle(Package *package, MYSQL *connection);
char* orderDrink(Package *package, MYSQL *connection);
char* checkDaysProfitForClub(Package *package, MYSQL *connection);

int order(MYSQL* connection, Article article, int quantityOrdered, int deleteArticleFlag, int clubId);
int insertNewClub(MYSQL* connection, char *clubName);
int insertNewArticle(MYSQL* connection, Article article, int clubId);
int updateArticle(MYSQL* connection, Article article, int clubId);
int deleteArticle(MYSQL* connection, Article article);
int checkIfClubExists(char *clubName);

Article* checkIfArticleExistsInTheClub(MYSQL* connection, Article article, int clubId);




