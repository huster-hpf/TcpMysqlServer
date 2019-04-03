// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "Timer.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include <sys/epoll.h>


class EventLoop;
class HttpData;


class Channel
{
private:
    typedef std::function<void()> CallBack;    //回调函数
    EventLoop *loop_;               //channel 所属enentloop对象
    int fd_;                        //关注连接的文件描述符
    __uint32_t events_;             //关注的事件（http请求和响应）
    __uint32_t revents_;            //poll/epoll返回的事件
    __uint32_t lastEvents_;			//????

    // 方便找到上层持有该Channel的对象
    std::weak_ptr<HttpData> holder_;   //关注HttpData对象的请求和响应

private:
    int parse_URI();
    int parse_Headers();
    int analysisRequest();

    CallBack readHandler_;     //读事件回调
    CallBack writeHandler_;    //写事件回调
    CallBack errorHandler_;    //错误事件回调
    CallBack connHandler_;     //连接事件回调

public:
    Channel(EventLoop *loop);    
    Channel(EventLoop *loop, int fd);
    ~Channel();
    int getFd();              //获取关注事件（连接）的文件描述符
    void setFd(int fd);       //设置关注事件(连接）的文件描述符

    void setHolder(std::shared_ptr<HttpData> holder)    //设置通道关注的http请求和响应
    {
        holder_ = holder;
    }
    std::shared_ptr<HttpData> getHolder()               //返回通道关注的http请求和响应
    {
        std::shared_ptr<HttpData> ret(holder_.lock());
        return ret;
    }

    void setReadHandler(CallBack &&readHandler)      //设置读事件回调
    {
        readHandler_ = readHandler;
    }
    void setWriteHandler(CallBack &&writeHandler)    //设置写事件回调
    {
        writeHandler_ = writeHandler;
    }
    void setErrorHandler(CallBack &&errorHandler)    //设置错误事件回调
    {
        errorHandler_ = errorHandler;
    }
    void setConnHandler(CallBack &&connHandler)      //设置连接事件回调
    {
        connHandler_ = connHandler;
    }

    void handleEvents()                 //处理事件（http请求和响应）
    {
        events_ = 0;
        if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
        {
            events_ = 0;
            return;
        }
        if (revents_ & EPOLLERR)
        {
            if (errorHandler_) errorHandler_();
            events_ = 0;
            return;
        }
        if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
        {
            handleRead();			//处理读请求
        }
        if (revents_ & EPOLLOUT)	
        {
            handleWrite();			//处理写请求
        }
        handleConn();				//处理连接请求
    }
    void handleRead();
    void handleWrite();
    void handleError(int fd, int err_num, std::string short_msg);
    void handleConn();

    void setRevents(__uint32_t ev)		//设置关注事件（http请求和响应）
    {
        revents_ = ev;
    }

    void setEvents(__uint32_t ev)
    {
        events_ = ev;
    }
    __uint32_t& getEvents()
    {
        return events_;
    }

    bool EqualAndUpdateLastEvents()
    {
        bool ret = (lastEvents_ == events_);
        lastEvents_ = events_;
        return ret;
    }

    __uint32_t getLastEvents()
    {
        return lastEvents_;
    }

};

typedef std::shared_ptr<Channel> SP_Channel;	//通道的智能指针