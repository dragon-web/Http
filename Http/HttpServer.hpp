#pragma once

#include<iostream>
#include<pthread.h>
#include"Sock.hpp"
#include"Log.hpp"
#include"Protocol.hpp"
#define DefaultPort 80
class HttpServer
{
  private:
  int listen_sock;
  int port;
  public:
  HttpServer(int _port = DefaultPort) : port(_port),listen_sock(-1)
  {
  }
  ~HttpServer()
  {
    if(listen_sock > 0)
    {
      close(listen_sock);
    }
  }
  void InitServer()
  {
    listen_sock = Sock::Socket();
    Sock::Bind(listen_sock,port);
    Sock::Listen(listen_sock);
  }
  void Start()
  {
    LOG(Normal,"Server start success!");
    for(;;)
    {
      int sock = Sock::Accept(listen_sock);
      if(sock >= 0)
      {
        LOG(Normal ,"get a new linking!");
        pthread_t tid;
        pthread_create(&tid ,nullptr,Entry::HanderRequest,(void*)sock);
        pthread_detach(tid);
      }
      
     LOG(Normal,"for test ");
     sleep(1);
    }

  }
};
