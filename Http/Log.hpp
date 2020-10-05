
#pragma once
#include<iostream>
#include<string>
#include<sys/time.h>
//Normal Error

#define ARG_ERR 1
#define SOCKET_CREATE 2
#define BIND_ERROR 3
#define LISTEN_ERR 4


// log error level
#define Normal 1
#define Warning 2
#define Error 3
#define Fatal 4

//back log 

#define NUM 5


uint64_t GetTimeStamp()
{
  struct timeval _time; 
  if(gettimeofday(&_time,nullptr) == 0)
  {
    return _time.tv_sec;
  }
  return 0;
}


static std::string log_level[] = 
{
  "Unknow",
  "Normal",
  "Warning",
  "Error",
  "Fatal",
};

//日志包含   错误级别   时间戳  模块名称 消息内容 打印行号 
void Log(int level,std::string msg,std::string model,int number)
{
  if (level < 1 || level > 4)
  {
    level = 0;
  }
  std::cout<<"[ "<<log_level[level]<<" ][" << msg<<" ][" << GetTimeStamp()<<"][" << model << "]["<<number<<"]"<<std::endl;
}

#define LOG(ERR_LEVEL,MESSAGE)\
  Log(ERR_LEVEL,MESSAGE,__FILE__,__LINE__)


//如果宏参带#，将表示将宏标识作用变成字符串











