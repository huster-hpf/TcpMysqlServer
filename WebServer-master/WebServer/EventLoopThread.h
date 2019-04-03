// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "base/Condition.h"
#include "base/MutexLock.h"
#include "base/Thread.h"
#include "base/noncopyable.h"
#include "EventLoop.h"

class EventLoopThread :noncopyable
{
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop* startLoop();			//开启事件循环线程

private:
    void threadFunc();				//线程回调函数
    EventLoop *loop_;				//事件循环对象
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};