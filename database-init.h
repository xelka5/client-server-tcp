
#define CREATE_CLUBS_TABLE "CREATE TABLE club(id INT AUTO_INCREMENT, name VARCHAR(255), PRIMARY KEY (id))"
#define DROP_CLUBS_TABLE "DROP TABLE IF EXISTS club"
#define CREATE_ARTICLE_TABLE "CREATE TABLE article(id INT AUTO_INCREMENT, name VARCHAR(255), price DECIMAL, quantity INT, clubId INT, PRIMARY KEY (id))"
#define DROP_ARTICLE_TABLE "DROP TABLE IF EXISTS article"
#define CREATE_ORDERS_TABLE "CREATE TABLE orders(id INT AUTO_INCREMENT, articleId INT, quantity INT, orderDate DATE, PRIMARY KEY (id))"
#define DROP_ORDERS_TABLE "DROP TABLE IF EXISTS orders"

void createDbTables(MYSQL* connection);
void seedDbTables(MYSQL* connection);

