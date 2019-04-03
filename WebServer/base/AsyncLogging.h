// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "CountDownLatch.h"
#include "MutexLock.h"
#include "Thread.h"
#include "LogStream.h"
#include "noncopyable.h"
#include <functional>
#include <string>
#include <vector>

class AsyncLogging : noncopyable
{
public:
    AsyncLogging(const std::string basename, int flushInterval = 2);
    ~AsyncLogging()
    {
        if (running_)
            stop();
    }
    void append(const char* logline, int len);

    void start()			//开启日志线程
    {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop()				//关闭日志线程
    {
        running_ = false;
        cond_.notify();
        thread_.join();
    }


private:
    void threadFunc();
    typedef FixedBuffer<kLargeBuffer> Buffer;					//buffer类
    typedef std::vector<std::shared_ptr<Buffer>> BufferVector;	//buffer容器类
    typedef std::shared_ptr<Buffer> BufferPtr;
    const int flushInterval_;	//缓冲区刷新时间间隔(s)
    bool running_;				//日志文件线程开启表示
    std::string basename_;		//日志文件名
    Thread thread_;				//日志线程
    MutexLock mutex_;			//互斥锁
    Condition cond_;			//条件变量
    BufferPtr currentBuffer_;	//当前使用缓冲区指针
    BufferPtr nextBuffer_;		//可替换使用缓冲区指针
    BufferVector buffers_;		//buffer容器对象
    CountDownLatch latch_;
};