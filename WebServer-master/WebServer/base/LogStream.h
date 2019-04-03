// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "noncopyable.h"
#include <assert.h>
#include <string.h>
#include <string>


class AsyncLogging;
const int kSmallBuffer = 4000;		//缓冲区最小值
const int kLargeBuffer = 4000 * 1000;	//缓冲区最大值

template<int SIZE> 
class FixedBuffer: noncopyable
{
public:
    FixedBuffer()
    :   cur_(data_)
    { }

    ~FixedBuffer()
    { }

    void append(const char* buf, size_t len)
    {
        if (avail() > static_cast<int>(len))
        {
            memcpy(cur_, buf, len);		//buf数据拷贝到缓冲区（当前位置）
            cur_ += len;
        }
    }

    const char* data() const { return data_; }					//范围缓冲区数据
    int length() const { return static_cast<int>(cur_ - data_); }	//已使用缓冲区大小

    char* current() { return cur_; }	//返回缓冲区使用空间位置
    int avail() const { return static_cast<int>(end() - cur_); }	//缓冲区可用大小
    void add(size_t len) { cur_ += len; }				//缓冲区已使用数据前移

    void reset() { cur_ = data_; }			//缓冲区使用空间清零
    void bzero() { memset(data_, 0, sizeof data_); }	 //缓冲区全部清零


private:
    const char* end() const { return data_ + sizeof data_; }		//缓冲区末尾位置

    char data_[SIZE];		//数组数据
    char* cur_;				//缓冲区使用空间位置
};



class LogStream : noncopyable
{
public:
    typedef FixedBuffer<kSmallBuffer> Buffer;

    LogStream& operator<<(bool v)
    {
        buffer_.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);

    LogStream& operator<<(const void*);

    LogStream& operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }
    LogStream& operator<<(double);
    LogStream& operator<<(long double);

    LogStream& operator<<(char v)
    {
        buffer_.append(&v, 1);
        return *this;
    }

    LogStream& operator<<(const char* str)
    {
        if (str)
            buffer_.append(str, strlen(str));
        else
            buffer_.append("(null)", 6);
        return *this;
    }

    LogStream& operator<<(const unsigned char* str)
    {
        return operator<<(reinterpret_cast<const char*>(str));
    }

    LogStream& operator<<(const std::string& v)
    {
        buffer_.append(v.c_str(), v.size());
        return *this;
    }

    void append(const char* data, int len) { buffer_.append(data, len); }
    const Buffer& buffer() const { return buffer_; }
    void resetBuffer() { buffer_.reset(); }		//重置缓冲区

private:
    void staticCheck();

    template<typename T>
    void formatInteger(T);

    Buffer buffer_;

    static const int kMaxNumericSize = 32;
};