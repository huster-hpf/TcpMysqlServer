// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_POLLER_POLLPOLLER_H
#define MUDUO_NET_POLLER_POLLPOLLER_H

#include <muduo/net/Poller.h>

#include <map>
#include <vector>

struct pollfd;          

namespace muduo
{
namespace net
{

///
/// IO Multiplexing with poll(2).
///
class PollPoller : public Poller
{
 public:

  PollPoller(EventLoop* loop);
  virtual ~PollPoller();

  virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels);       //处理活跃事件列表
  virtual void updateChannel(Channel* channel);                             //注册、更行活跃事件
  virtual void removeChannel(Channel* channel);                             //移除活跃通道

 private:
  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;

  typedef std::vector<struct pollfd> PollFdList;            //关注事件的文件描述符集合
  typedef std::map<int, Channel*> ChannelMap;	// key是文件描述符，value是Channel*
  PollFdList pollfds_;                              //文件描述符
  ChannelMap channels_;                             //活跃事件通道集合
};

}
}
#endif  // MUDUO_NET_POLLER_POLLPOLLER_H
