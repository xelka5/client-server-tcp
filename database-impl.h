#include "common.h"

#define MAX_QUERY_SIZE 1000
#define UPDATE_ARTICLE_FLAG 0
#define DELETE_ARTICLE_FLAG 1

char* fillNewArticle(Package* package);
char* orderDrink(Package* package);
char* checkDaysProfitForClub(Package *package);
int order(MYSQL* connection, Article article, int quantityOrdered, int deleteArticleFlag, int clubId);
int insertNewClub(MYSQL* connection, char *clubName);
int insertNewArticle(MYSQL* connection, Article article, int clubId);
int updateArticle(MYSQL* connection, Article article, int clubId);
int deleteArticle(MYSQL* connection, Article article);
Article* checkIfArticleExistsInTheClub(MYSQL* connection, Article article, int clubId);
int checkIfClubExists(char *clubName);




