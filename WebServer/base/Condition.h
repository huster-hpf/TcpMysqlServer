// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "noncopyable.h"
#include "MutexLock.h"
#include <pthread.h>
#include <pthread.h>
#include <errno.h>
#include <cstdint>
#include <time.h>

class Condition: noncopyable
{
public:
    explicit Condition(MutexLock &_mutex):
        mutex(_mutex)
    {
        pthread_cond_init(&cond, NULL);			//初始化
    }
    ~Condition()
    {
        pthread_cond_destroy(&cond);			//销毁
    }
    void wait()									//线程等待
    {
        pthread_cond_wait(&cond, mutex.get());
    }
    void notify()								//通知，一对一
    {
        pthread_cond_signal(&cond);
    }
    void notifyAll()							//广播
    {
        pthread_cond_broadcast(&cond);
    }
    bool waitForSeconds(int seconds)			//等待一定时间
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
    }
private:
    MutexLock &mutex;
    pthread_cond_t cond;
};