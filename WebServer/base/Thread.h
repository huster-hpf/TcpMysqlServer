// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "CountDownLatch.h"
#include "noncopyable.h"
#include <functional>
#include <memory>
#include <pthread.h>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>


class Thread : noncopyable
{
public:
    typedef std::function<void ()> ThreadFunc;		//函数对象类，封装可调用对象
													//（普通函数、lambda表达式、函数指针、以及其他函数对象等）
    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();
    void start();
    int join();										//等待线程结束，实现线程同步
    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    const std::string& name() const { return name_; }

private:
    void setDefaultName();		//设置线程名
    bool started_;				//线程开启标志
    bool joined_;				//等待线程结束，并且继续执行
    pthread_t pthreadId_;		//进程中独立线程id（可能相同）
    pid_t tid_;					//线程真实ID
    ThreadFunc func_;			//线程回调函数
    std::string name_   		//线程名
    CountDownLatch latch_;		//封装条件变量类，用于起跑
};