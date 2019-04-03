// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "EventLoop.h"
#include "Channel.h"
#include "EventLoopThreadPool.h"
#include <memory>


class Server
{
public:
    Server(EventLoop *loop, int threadNum, int port);
    ~Server() {}
    EventLoop* getLoop() const { return loop_; }	//返回事件循环对象
    void start();							//启动服务端服务
    void handNewConn();
    void handThisConn() { loop_->updatePoller(acceptChannel_); }

private:
    EventLoop *loop_;
    int threadNum_;
    std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
    bool started_;
    std::shared_ptr<Channel> acceptChannel_;		
    int port_;
    int listenFd_;					//监听文件描述符
    static const int MAXFDS = 100000;		//最大连接数
};