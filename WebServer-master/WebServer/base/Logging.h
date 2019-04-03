// @Author hong pengfei
// @Email 13260623635@163.com
#pragma once
#include "LogStream.h"
#include <pthread.h>
#include <string.h>
#include <string>
#include <stdio.h>

//Logging是对外接口，Logging类内涵一个LogStream对象，
//主要是为了每次打log的时候在log之前和之后加上固定的格式化的信息，
//比如打log的行、文件名等信息。

class AsyncLogging;


class Logger
{
public:
    Logger(const char *fileName, int line);
    ~Logger();
    LogStream& stream() { return impl_.stream_; }			//返回LogStream对象

    static void setLogFileName(std::string fileName)		//设置文件名
    {
        logFileName_ = fileName;
    }
    static std::string getLogFileName()				//返回文件名
    {
        return logFileName_;
    }

private:
    class Impl
    {
    public:
        Impl(const char *fileName, int line);
        void formatTime();			//格式化输出日志信息

        LogStream stream_;			//LogStream主要用来格式化输出
        int line_;
        std::string basename_;			
    };
    Impl impl_;
    static std::string logFileName_;	 	//日志文件名
};

#define LOG Logger(__FILE__, __LINE__).stream()