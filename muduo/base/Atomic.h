// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace muduo
{

namespace detail
{
template<typename T>
class AtomicIntegerT : boost::noncopyable
{
 public:
  AtomicIntegerT()
    : value_(0)
  {
  }

  // uncomment if you need copying and assignment
  //
  // AtomicIntegerT(const AtomicIntegerT& that)
  //   : value_(that.get())
  // {}
  //
  // AtomicIntegerT& operator=(const AtomicIntegerT& that)
  // {
  //   getAndSet(that.get());
  //   return *this;
  // }

  T get()           //对原子性操作函数进行封装
  {
    return __sync_val_compare_and_swap(&value_, 0, 0);
  }

  T getAndAdd(T x)      //对原子性操作函数进行封装,返回加前值
  {
    return __sync_fetch_and_add(&value_, x);
  }

  T addAndGet(T x)          //返回加后值
  {
    return getAndAdd(x) + x;
  }

  T incrementAndGet()       //++i
  {
    return addAndGet(1);
  }

  T decrementAndGet()       //--i
  {
    return addAndGet(-1);
  }

  void add(T x)
  {
    getAndAdd(x);
  }

  void increment()
  {
    incrementAndGet();
  }

  void decrement()
  {
    decrementAndGet();
  }

  T getAndSet(T newValue)
  {
    return __sync_lock_test_and_set(&value_, newValue);
  }

 private:
  volatile T value_;    //放置编译器优化
};
}

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif  // MUDUO_BASE_ATOMIC_H
