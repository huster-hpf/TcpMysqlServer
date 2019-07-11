// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include <muduo/base/Atomic.h>
#include <muduo/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo
{

class Thread : boost::noncopyable
{
 public:
  typedef boost::function<void ()> ThreadFunc;      //用于绑定可调用对象

  explicit Thread(const ThreadFunc&, const string& name = string());    //构造
  ~Thread();                //析构

  void start();             //开启线程
  int join(); // return pthread_join()      //主线程等到子线程结束，回收资源

  bool started() const { return started_; }     //线程是否开启
  // pthread_t pthreadId() const { return pthreadId_; }
  pid_t tid() const { return tid_; }        //返回线程真实ID
  const string& name() const { return name_; }      //返回线程名

  static int numCreated() { return numCreated_.get(); }     //返回创建线程数

 private:
  static void* startThread(void* thread);       
  void runInThread();

  bool       started_;      //线程开启标志
  pthread_t  pthreadId_;     //pthread_t给pthread_XXX函数使用
  pid_t      tid_;              //pid_t作为线程标识
                                //pthreads只能保证同一进程内，同一时刻的各个线程不同；不能保证同一个进程全程时段每个线程具有不同的id，不能保证线程id的唯一性。
  ThreadFunc func_;                 //线程回调函数
  string     name_;                     //线程名

  static AtomicInt32 numCreated_;       //创建线程数（原子性操作提供线程安全）
};

}
#endif
