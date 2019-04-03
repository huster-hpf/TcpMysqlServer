// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

// CountDownLatch的主要作用是确保Thread中传进去的func真的启动了以后
// 外层的start才返回
class CountDownLatch : noncopyable			//对condition类进行封装，
											//倒计时未完，CountDownLatch的内部一直处于Condition类的wait（）状态。
											//倒计时完毕，唤醒cond,程序正常执行。
{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();

private:
    mutable MutexLock mutex_;		//一把 锁、条件变量和计数器构成
    Condition condition_;
    int count_;
};