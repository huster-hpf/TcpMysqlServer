#include <src/muduo/base/Types.h>
#include <mysql/mysql.h>

#define IP "rm-bp137n5t350n8r4jp.mysql.rds.aliyuncs.com"
#define PORT 3306
#define USR "root"
#define PSW "DTStest1234"
#define DB "test1000"


using namespace muduo;


class MysqlServer
{
public:
  MysqlServer();
  ~MysqlServer();
  string process(char *sql_);
private:
  MYSQL *mysqlconn_;
};

