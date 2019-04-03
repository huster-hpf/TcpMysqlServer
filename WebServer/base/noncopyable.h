// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once

class noncopyable			//禁止拷贝类。   有啥用捏？？？
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};