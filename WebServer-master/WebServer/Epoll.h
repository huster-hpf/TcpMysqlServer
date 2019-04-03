// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "Channel.h"
#include "HttpData.h"
#include "Timer.h"
#include <vector>
#include <unordered_map>
#include <sys/epoll.h>
#include <memory>

class Epoll
{
public:
    Epoll();
    ~Epoll();
    void epoll_add(SP_Channel request, int timeout);		//添加活动通道请求
    void epoll_mod(SP_Channel request, int timeout);
    void epoll_del(SP_Channel request);						//删除活动通道请求
    std::vector<std::shared_ptr<Channel>> poll();
    std::vector<std::shared_ptr<Channel>> getEventsRequest(int events_num);
    void add_timer(std::shared_ptr<Channel> request_data, int timeout);
    int getEpollFd()    //获取epoll文件描述符
    {
        return epollFd_;
    }
    void handleExpired();
private:
    static const int MAXFDS = 100000;    //最大连接数（http请求）
    int epollFd_;        //epoll文件描述符
    std::vector<epoll_event> events_;   
    std::shared_ptr<Channel> fd2chan_[MAXFDS];   //关注通道，对应文件描述符
    std::shared_ptr<HttpData> fd2http_[MAXFDS];  //关注的http请求和响应，对应文件描述符
    TimerManager timerManager_;    //定时器对象
};