// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once     //避免同一文件被include多次
#include "noncopyable.h"
#include <pthread.h>
#include <cstdio>

class MutexLock: noncopyable      //互斥锁，封装临界区
{
public:
    MutexLock()
    {
        pthread_mutex_init(&mutex, NULL);
    }
    ~MutexLock()
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }
    void lock()
    {
        pthread_mutex_lock(&mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_t *get()
    {
        return &mutex;
    }
private:
    pthread_mutex_t mutex;		//pthread_mutex_t是互斥变量使用特定的数据类型

// 友元类不受访问权限影响
private:
    friend class Condition;		//C++中的friend关键字其实做这样的事情：
								//在一个类中指明其他的类（或者）函数能够直接访问该类中的private和protected成员
};


class MutexLockGuard: noncopyable         //RAII手法封装互斥器的创建与销毁。
									      //(资源的地点是构造函数，释放点是析构函数）
{
public:
    explicit MutexLockGuard(MutexLock &_mutex):
    mutex(_mutex)
    {
        mutex.lock();
    }
    ~MutexLockGuard()
    {
        mutex.unlock();
    }
private:
    MutexLock &mutex;				//互斥量成员
};