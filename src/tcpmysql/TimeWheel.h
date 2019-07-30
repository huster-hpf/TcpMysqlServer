#include <unordered_map>
#include <unordered_set>
#include <boost/circular_buffer.hpp>
#include <src/muduo/base/noncopyable.h>
#include <src/muduo/base/Types.h>

typedef std::weak_ptr<std::pair<string,string>> WeakSqlPtr;
struct SqlStmt : public muduo::copyable
{
  explicit SqlStmt(const WeakSqlPtr &weaksql):weaksql_(weaksql)
  {
  }

  SqlStmt()
  {
  }

  ~SqlStmt()
  {
  }

  WeakSqlPtr weaksql_;
};

typedef std::shared_ptr<SqlStmt> SqlStmtPtr;
typedef std::weak_ptr<SqlStmt> WeakSqlStmtPtr;
typedef std::unordered_set<SqlStmtPtr> Bucket;
typedef boost::circular_buffer<Bucket> SqlStmtList;
typedef std::unordered_map<string,WeakSqlStmtPtr> SqlCheckList;
