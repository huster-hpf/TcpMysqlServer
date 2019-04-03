// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "base/Thread.h"
#include "Epoll.h"
#include "base/Logging.h"
#include "Channel.h"
#include "base/CurrentThread.h"
#include "Util.h"
#include <vector>
#include <memory>
#include <functional>

#include <iostream>
using namespace std;


class EventLoop
{
public:
    typedef std::function<void()> Functor;     //https://www.cnblogs.com/diegodu/p/6180350.html   可调用的std::function对象
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void runInLoop(Functor&& cb);   //keynote
    void queueInLoop(Functor&& cb);     //keynote
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }    //判断初始化eventloop的线程是不是当前运行函数的线程
    void assertInLoopThread()
    {
        assert(isInLoopThread());
    }
    void shutdown(shared_ptr<Channel> channel)	//关闭关注通道
    {
        shutDownWR(channel->getFd());
    }
    void removeFromPoller(shared_ptr<Channel> channel)	//移除关注通道
    {
        //shutDownWR(channel->getFd());
        poller_->epoll_del(channel);
    }
    void updatePoller(shared_ptr<Channel> channel, int timeout = 0)	//更新关注通道
    {
        poller_->epoll_mod(channel, timeout);
    }
    void addToPoller(shared_ptr<Channel> channel, int timeout = 0)	//添加关注通道
    {
        poller_->epoll_add(channel, timeout);
    }
    
private:
    // 声明顺序 wakeupFd_ > pwakeupChannel_
    bool looping_;     //启动事件循环标志
    shared_ptr<Epoll> poller_;    //keynote
    int wakeupFd_;    //eventfd，是eventloop主动唤醒epoll的fd
    bool quit_;       //终止事件循环标志
    bool eventHandling_;     //时间处理标志
    mutable MutexLock mutex_;     //mutable是为了突破const的限制而设置的。
								  //被mutable修饰的变量，将永远处于可变的状态，
								  //即使在一个const函数中，甚至结构体变量或者类对象为const，
								  //其mutable成员也可以被修改.mutable在类中只能够修饰非静态数据成员.
    std::vector<Functor> pendingFunctors_;  //添加到eventloop中需要执行的函数对象
    bool callingPendingFunctors_;           //调用eventloop的需要执行的函数标志
    const pid_t threadId_;                  //初始化时候,本eventloop 所在的线程ID。
    shared_ptr<Channel> pwakeupChannel_;    //封装eventfd的channel
    
    void wakeup();       	// wakeup函数用来显示的唤醒 epoll ,主要是在 eventloop执行了quit
							// 还有添加了函数对象到队列中的时候 主动的唤醒 epoll
							// 能唤醒的原因在于,eventloop 对象持有一个 eventfd,注册在了epoll中
							// 然后需要唤醒的时候,直接 eventfd 中写数据即可.
	
    void handleRead();      //eventloop 主动唤醒 epoll 处理eventfd 可读事件。
    void doPendingFunctors();    //执行队列中的函数对象
    void handleConn();      //处理连接到来事件
};
