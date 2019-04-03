// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "base/noncopyable.h"
#include "EventLoopThread.h"
#include "base/Logging.h"
#include <memory>
#include <vector>

class EventLoopThreadPool : noncopyable
{
public:
    EventLoopThreadPool(EventLoop* baseLoop, int numThreads);

    ~EventLoopThreadPool()
    {
        LOG << "~EventLoopThreadPool()";
    }
    void start();

    EventLoop *getNextLoop();

private:
    EventLoop* baseLoop_;										//主事件循环
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;		//事件循环线程   线程池数组
    std::vector<EventLoop*> loops_;								//事件循环对象   数组
};