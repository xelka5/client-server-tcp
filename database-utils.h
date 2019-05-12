#define DB_NAME "clubs"
#define HOST "127.0.0.1"
#define USERNAME "root"
#define PASSWORD "root"

MYSQL* connectToMysql();
void disconnectFromMysql(MYSQL* connection);
void finish_with_error(MYSQL *connection);
char* get_error_message(MYSQL *connection);
