// @Author hong pengfei
// @Email 13260623635@163.com
#include "EventLoop.h"
#include "base/Logging.h"
#include "Util.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <iostream>
using namespace std;

__thread EventLoop* t_loopInThisThread = 0;   //事件循环指针对象，一个eventloop对象都有一份。
			                                  //https://www.cnblogs.com/dawnworld/articles/4092657.html

int createEventfd()    //产生eventfd，监听时间的文件描述符
{
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG << "Failed in eventfd";
        abort();
    }
    return evtfd;
}

EventLoop::EventLoop()
:   looping_(false),
    poller_(new Epoll()),
    wakeupFd_(createEventfd()),
    quit_(false),
    eventHandling_(false),
    callingPendingFunctors_(false),
    threadId_(CurrentThread::tid()),
    pwakeupChannel_(new Channel(this, wakeupFd_))		//唤醒的通道
{
    if (t_loopInThisThread)
    {
        //LOG << "Another EventLoop " << t_loopInThisThread << " exists in this thread " << threadId_;
    }
    else
    {
        t_loopInThisThread = this;
    }
    //pwakeupChannel_->setEvents(EPOLLIN | EPOLLET | EPOLLONESHOT);
    pwakeupChannel_->setEvents(EPOLLIN | EPOLLET);  						//这是？？？
    pwakeupChannel_->setReadHandler(bind(&EventLoop::handleRead, this));    //设置处理可读事件回调
    pwakeupChannel_->setConnHandler(bind(&EventLoop::handleConn, this));    //设置连接到来事件回调
    poller_->epoll_add(pwakeupChannel_, 0);    //epool中添加监听通道
}

void EventLoop::handleConn()
{
    //poller_->epoll_mod(wakeupFd_, pwakeupChannel_, (EPOLLIN | EPOLLET | EPOLLONESHOT), 0);
    updatePoller(pwakeupChannel_, 0);
}


EventLoop::~EventLoop()
{
    //wakeupChannel_->disableAll();
    //wakeupChannel_->remove();
    close(wakeupFd_);
    t_loopInThisThread = NULL;
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = writen(wakeupFd_, (char*)(&one), sizeof one);
    if (n != sizeof one)
    {
        LOG<< "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = readn(wakeupFd_, &one, sizeof one);
    if (n != sizeof one)
    {
        LOG << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
    //pwakeupChannel_->setEvents(EPOLLIN | EPOLLET | EPOLLONESHOT);
    pwakeupChannel_->setEvents(EPOLLIN | EPOLLET);
}

void EventLoop::runInLoop(Functor&& cb)
{
    if (isInLoopThread())
        cb();
    else
        queueInLoop(std::move(cb));
}

void EventLoop::queueInLoop(Functor&& cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.emplace_back(std::move(cb));
    }

    if (!isInLoopThread() || callingPendingFunctors_)
        wakeup();
}

void EventLoop::loop()
{
    assert(!looping_);
    assert(isInLoopThread());
    looping_ = true;
    quit_ = false;
    //LOG_TRACE << "EventLoop " << this << " start looping";
    std::vector<SP_Channel> ret;	//活动通道容器
    while (!quit_)
    {
        //cout << "doing" << endl;
        ret.clear();
        ret = poller_->poll();    	//返回活动通道
        eventHandling_ = true;
        for (auto &it : ret)
            it->handleEvents();		//处理可读事件
        eventHandling_ = false;
        doPendingFunctors();         //执行队列中的函数对象
        poller_->handleExpired();   //处理到期事件
    }
    looping_ = false;
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);    //vector中swap使用，swap可以有效降低当前 vector占用的内存总量。http
										    //https://baijiahao.baidu.com/s?id=1610227871099894962&wfr=spider&for=pc
    }

    for (size_t i = 0; i < functors.size(); ++i)
        functors[i]();
    callingPendingFunctors_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
    if (!isInLoopThread())
    {
        wakeup();
    }
}