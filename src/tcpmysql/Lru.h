#include <src/muduo/net/TcpConnection.h>
#include <list>
#include <unordered_map>
#define SIZE 1000


//typedef std::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;

//struct Lru : public muduo::copyable
{
  explicit Lru(const WeakTcpConnectionPtr &weakconn):weakconn_(weakconn)
  {
  }

  ~Lru()
  {
    muduo::net::TcpConnectionPtr conn=weakconn_.lock();
    if(conn)
    {
      conn->shutdown();
    }
  }
  
  WeakTcpConnectionPtr weakconn_;
};

typedef std::list<Lru> LruList;
typedef LruList::iterator Iter;
typedef std::unordered_map<Lru,Iter> LruMap;
