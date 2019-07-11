#ifndef MUDUO_BASE_LOGSTREAM_H
#define MUDUO_BASE_LOGSTREAM_H

#include <muduo/base/StringPiece.h>
#include <muduo/base/Types.h>
#include <assert.h>
#include <string.h> // memcpy
#ifndef MUDUO_STD_STRING
#include <string>
#endif
#include <boost/noncopyable.hpp>

namespace muduo
{

namespace detail
{

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000*1000;

template<int SIZE>
class FixedBuffer : boost::noncopyable
{
 public:
  FixedBuffer()
    : cur_(data_)
  {
    setCookie(cookieStart);
  }

  ~FixedBuffer()
  {
    setCookie(cookieEnd);
  }

  void append(const char* /*restrict*/ buf, size_t len)
  {
    // FIXME: append partially
    if (implicit_cast<size_t>(avail()) > len)
    {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char* data() const { return data_; }            //返回c风格字符串
  int length() const { return static_cast<int>(cur_ - data_); }     //返回data字节长度

  // write to data_ directly
  char* current() { return cur_; }
  int avail() const { return static_cast<int>(end() - cur_); }      //buff是否合法
  void add(size_t len) { cur_ += len; }                     //指针偏移量

  void reset() { cur_ = data_; }                    //清空data字节
  void bzero() { ::bzero(data_, sizeof data_); }    //data清零

  // for used by GDB
  const char* debugString();            //buffer清零
  void setCookie(void (*cookie)()) { cookie_ = cookie; }    //设置函数回调
  // for used by unit test
  string asString() const { return string(data_, length()); }   //以c++string形式返回字符串

 private:
  const char* end() const { return data_ + sizeof data_; }      //data尾部位置
  // Must be outline function for cookies.
  static void cookieStart();            
  static void cookieEnd();

  void (*cookie_)();    //设置回调函数
  char data_[SIZE];     //buffer中存放数据
  char* cur_;           //当前指针，指向数据buffer某一位置。
};

}

class LogStream : boost::noncopyable            //往buffer里面写数据
{
  typedef LogStream self;
 public:
  typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;         //传入常量kSmallBuffer，4kb大小buffer

  self& operator<<(bool v){ 
    buffer_.append(v ? "1" : "0", 1);
    return *this;
  }

  self& operator<<(short);          
  self& operator<<(unsigned short);
  self& operator<<(int);
  self& operator<<(unsigned int);
  self& operator<<(long);
  self& operator<<(unsigned long);
  self& operator<<(long long);
  self& operator<<(unsigned long long);

  self& operator<<(const void*);

  self& operator<<(float v)
  {
    *this << static_cast<double>(v);
    return *this;
  }
  self& operator<<(double);
  // self& operator<<(long double);

  self& operator<<(char v)
  {
    buffer_.append(&v, 1);
    return *this;
  }

  // self& operator<<(signed char);
  // self& operator<<(unsigned char);

  self& operator<<(const char* v)
  {
    buffer_.append(v, strlen(v));
    return *this;
  }

  self& operator<<(const string& v)
  {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

#ifndef MUDUO_STD_STRING
  self& operator<<(const std::string& v)
  {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }
#endif

  self& operator<<(const StringPiece& v)
  {
    buffer_.append(v.data(), v.size());
    return *this;
  }

  void append(const char* data, int len) { buffer_.append(data, len); }     //往buffer里面添加数据
  const Buffer& buffer() const { return buffer_; }          //返回buffer数据
  void resetBuffer() { buffer_.reset(); }               //清空buffer数据

 private:
  void staticCheck();

  template<typename T>
  void formatInteger(T);

  Buffer buffer_;

  static const int kMaxNumericSize = 32;
};

class Fmt // : boost::noncopyable
{
 public:
  template<typename T>
  Fmt(const char* fmt, T val);

  const char* data() const { return buf_; }
  int length() const { return length_; }

 private:
  char buf_[32];
  int length_;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
{
  s.append(fmt.data(), fmt.length());
  return s;
}

}
#endif  // MUDUO_BASE_LOGSTREAM_H

