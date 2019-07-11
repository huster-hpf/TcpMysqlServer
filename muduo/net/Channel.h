// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <muduo/base/Timestamp.h>

namespace muduo
{
namespace net
{

class EventLoop;

///
/// A selectable I/O channel.
///
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd
class Channel : boost::noncopyable
{
 public:
  typedef boost::function<void()> EventCallback;            //时间回调函数
  typedef boost::function<void(Timestamp)> ReadEventCallback;   //可读事件

  Channel(EventLoop* loop, int fd);
  ~Channel();

  void handleEvent(Timestamp receiveTime);          //处理事件
  void setReadCallback(const ReadEventCallback& cb)     //设置可读事件回调
  { readCallback_ = cb; }
  void setWriteCallback(const EventCallback& cb)        //设置写事件回调
  { writeCallback_ = cb; }
  void setCloseCallback(const EventCallback& cb)        //设置关闭连接回调
  { closeCallback_ = cb; }
  void setErrorCallback(const EventCallback& cb)        //设置错误事件回调
  { errorCallback_ = cb; }

  /// Tie this channel to the owner object managed by shared_ptr,
  /// prevent the owner object being destroyed in handleEvent.
  void tie(const boost::shared_ptr<void>&);

  int fd() const { return fd_; }                        //返回通道监听文件描述符
  int events() const { return events_; }                //返回通道关注时间
  void set_revents(int revt) { revents_ = revt; } // used by pollers
  // int revents() const { return revents_; }
  bool isNoneEvent() const { return events_ == kNoneEvent; }   //

  void enableReading() { events_ |= kReadEvent; update(); }     //可读判断到来
  // void disableReading() { events_ &= ~kReadEvent; update(); }    
  void enableWriting() { events_ |= kWriteEvent; update(); }    //可写到来
  void disableWriting() { events_ &= ~kWriteEvent; update(); }  //不可写判断
  void disableAll() { events_ = kNoneEvent; update(); }
  bool isWriting() const { return events_ & kWriteEvent; }     //是否写判断

  // for Poller
  int index() { return index_; }                    //返回索引
  void set_index(int idx) { index_ = idx; }         //设置索引

  // for debug
  string reventsToString() const;                   

  void doNotLogHup() { logHup_ = false; }

  EventLoop* ownerLoop() { return loop_; }          //所属EventLoop
  void remove();

 private:
  void update();
  void handleEventWithGuard(Timestamp receiveTime);

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop* loop_;			// 所属EventLoop
  const int  fd_;			// 文件描述符，但不负责关闭该文件描述符
  int        events_;		// 关注的事件
  int        revents_;		// poll/epoll返回的事件
  int        index_;		// used by Poller.表示在poll的事件数组中的序号
  bool       logHup_;		// for POLLHUP

  boost::weak_ptr<void> tie_;
  bool tied_;
  bool eventHandling_;		// 是否处于处理事件中
  ReadEventCallback readCallback_;  //可读事件回调函数
  EventCallback writeCallback_;     //可写事件回调函数
  EventCallback closeCallback_;     //关闭通道事件回调函数
  EventCallback errorCallback_;     //错误事件回调函数
};

}
}
#endif  // MUDUO_NET_CHANNEL_H
