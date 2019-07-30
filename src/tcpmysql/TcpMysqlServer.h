#include <src/muduo/net/TcpServer.h>
#include <src/tcpmysql/MysqlServer.cc>
#include <boost/circular_buffer.hpp>
#include <src/tcpmysql/TimeWheel.h>
#include <list>
#define SIZECONN 1000

using namespace muduo;
using namespace muduo::net;


typedef std::list<TcpConnectionPtr>::iterator Iter;
class TcpMysqlServer
{
 public:
  TcpMysqlServer(EventLoop* loop,
             const InetAddress& listenAddr,
             int idleSeconds);
  void start();

 private:
  void onConnection(const TcpConnectionPtr& conn);

  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 Timestamp time);
  void onTimer();
  TcpServer tcpserver_;
  MysqlServer mysqlserver_;
  
  std::list<TcpConnectionPtr> lrulist_;
  std::unordered_map<TcpConnectionPtr,Iter> lrumap_;
  SqlStmt sqlstmt_;
  SqlStmtList sqlstmtlist_;
  SqlCheckList sqlchecklist_;
};
