#include <src/tcpmysql/TcpMysqlServer.cc>

#include <src/muduo/base/Logging.h>
#include <src/muduo/net/EventLoop.h>
#include <mysql/mysql.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

int main()
{
  //MYSQL *conn;
  //conn=mysql_init(NULL);
  LOG_INFO << "pid = " << getpid();
  EventLoop loop;
  InetAddress listenAddr("120.27.227.80",2007);
  TcpMysqlServer server(&loop, listenAddr,5);
  server.start();
  loop.loop();
}

