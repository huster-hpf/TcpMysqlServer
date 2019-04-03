// @Author hong pengfei
// @Email 13260623635@163.com
#include "CountDownLatch.h"				

CountDownLatch::CountDownLatch(int count)		//条件变量用成员初始化
  : mutex_(),
    condition_(mutex_),
    count_(count)
{ }

void CountDownLatch::wait()						//啥时候调用该函数么？
{
    MutexLockGuard lock(mutex_);
    while (count_ > 0)							//等待计数值为0
        condition_.wait();	
}

void CountDownLatch::countDown()				//倒数。    啥时候调用该函数么？
{
    MutexLockGuard lock(mutex_);
    --count_;
    if (count_ == 0)
        condition_.notifyAll();
}