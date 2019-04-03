// @Author hong pengfei
// @Email 13260623635@163.com
#include "EventLoop.h"
#include "Server.h"
#include "base/Logging.h"
#include <getopt.h>
#include <string>

int main(int argc, char *argv[])
{
    int threadNum = 4;   //线程数
    int port = 80;       //端口号
    std::string logPath = "./WebServer.log";    //日志文件名
  
    // parse args
    int opt;                 //选项
    const char *str = "t:l:p:";       //参数选项、参数
    while ((opt = getopt(argc, argv, str))!= -1)      
    {
        switch (opt)
        {
            case 't':  //选项后面接线程池数目
            {
                threadNum = atoi(optarg);
                break;
            }
            case 'l':  //选项后面接日志文件名
            {
                logPath = optarg;    //日志文件名
                if (logPath.size() < 2 || optarg[0] != '/')
                {
                    printf("logPath should start with \"/\"\n");
                    abort();
                }
                break;
            }
            case 'p':   //选项后面接端口号
            {
                port = atoi(optarg);   //端口号
                break;
            }
            default: break;
        }
    }
    Logger::setLogFileName(logPath);   //设置日志文件名
    // STL库在多线程上应用 
    #ifndef _PTHREADS             //这是防止头文件的重复包含和编译
        LOG << "_PTHREADS is not defined !";
    #endif               
    EventLoop mainLoop;     //主事件循环
    Server myHTTPServer(&mainLoop, threadNum, port);   //定义服务端对象
    myHTTPServer.start();           //开启服务端服务程序
    mainLoop.loop();                  //驱动事件循环
    return 0;  
}
