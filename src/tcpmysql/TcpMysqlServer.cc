#include <src/tcpmysql/TcpMysqlServer.h>
#include <src/muduo/base/Logging.h>
#include <src/muduo/net/EventLoop.h>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using namespace muduo;
using namespace muduo::net;

TcpMysqlServer::TcpMysqlServer(EventLoop* loop,
                       const InetAddress& listenAddr,int idleSeconds)
  : tcpserver_(loop, listenAddr, "TcpMysqlServer"), sqlstmtlist_(idleSeconds)
{
  tcpserver_.setConnectionCallback(
      std::bind(&TcpMysqlServer::onConnection, this, _1));
  tcpserver_.setMessageCallback(
      std::bind(&TcpMysqlServer::onMessage, this, _1, _2, _3));
  loop->runEvery(1.0, std::bind(&TcpMysqlServer::onTimer, this));
  sqlstmtlist_.resize(idleSeconds);
}

void TcpMysqlServer::start()
{
  tcpserver_.start();
}

void TcpMysqlServer::onConnection(const TcpConnectionPtr& conn)
{
  LOG_INFO << "TcpMysqlServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
  
  if(lrulist_.size()>=SIZECONN)
  {
    if(lrulist_.back())
      lrulist_.back()->shutdown();
    lrumap_.erase(lrulist_.back());
    lrulist_.pop_back();
    lrulist_.push_front(conn);
    lrumap_[conn]=lrulist_.begin();
  }
  else
  {
    lrulist_.push_front(conn);
    lrumap_[conn]=lrulist_.begin();
  }

}

void TcpMysqlServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp time)
{
  string msg(buf->retrieveAllAsString());
 // LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
 //          << "data received at " << time.toString();
  
  lrulist_.splice(lrulist_.begin(),lrulist_,lrumap_[conn]);
  lrumap_[conn]=lrulist_.begin();

  string echo;
  if(sqlchecklist_.find(msg)!=sqlchecklist_.end())
  {
    SqlStmtPtr sqlstmtptr((sqlchecklist_[msg]).lock());
    echo=(sqlstmtptr->weaksql_).lock()->second;
    if(sqlstmtptr!=NULL)
      sqlstmtlist_.back().insert(sqlstmtptr);
  }
  else
  {
    echo=mysqlserver_.process(const_cast<char *>(msg.c_str()));
    std::shared_ptr<std::pair<string,string>> sqlptr(new std::pair<string,string>({msg,echo}));
    WeakSqlStmtPtr weaksqlstmtptr=boost::any_cast<WeakSqlStmtPtr>(sqlptr);
    SqlStmtPtr sqlstmtptr=weaksqlstmtptr.lock();
    sqlchecklist_[msg]=weaksqlstmtptr;
    if(sqlstmtptr!=NULL)
      sqlstmtlist_.back().insert(sqlstmtptr);
  }
  conn->send(echo);
}
void TcpMysqlServer::onTimer()
{
  for(auto a:sqlstmtlist_.front())
    sqlchecklist_.erase((a->weaksql_).lock()->first);
  sqlstmtlist_.push_back(Bucket());
}
