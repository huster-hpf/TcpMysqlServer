// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "HttpData.h"
#include "base/noncopyable.h"
#include "base/MutexLock.h"
#include <unistd.h>
#include <memory>
#include <queue>
#include <deque>

class HttpData;

class TimerNode
{
public:
    TimerNode(std::shared_ptr<HttpData> requestData, int timeout);
    ~TimerNode();
    TimerNode(TimerNode &tn);
    void update(int timeout);    //更新定时器到期时间
    bool isValid();       		 //判断时间是否合法
    void clearReq();             
    void setDeleted() { deleted_ = true; }
    bool isDeleted() const { return deleted_; }
    size_t getExpTime() const { return expiredTime_; } 

private:
    bool deleted_;           //是否删除
    size_t expiredTime_;     //到期时间
    std::shared_ptr<HttpData> SPHttpData;    //这个智能指针对象有啥用？
};

struct TimerCmp       //传递给priority_queue中			//到期时间比较
{
    bool operator()(std::shared_ptr<TimerNode> &a, std::shared_ptr<TimerNode> &b) const
    {
        return a->getExpTime() > b->getExpTime();
    }
};

class TimerManager
{
public:
    TimerManager();
    ~TimerManager();
    void addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout);
    void handleExpiredEvent();				//处理到期时间时间

private:
    typedef std::shared_ptr<TimerNode> SPTimerNode;
    std::priority_queue<SPTimerNode, std::deque<SPTimerNode>, TimerCmp> timerNodeQueue;		//定时器优先队列
    //MutexLock lock;
};