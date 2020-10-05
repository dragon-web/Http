#pragma once 
#include<iostream>
class Connect;
class HttpRequest;
class HttpResponse;
class Entry
{ 
  public:
   static void *HanderRequest(void *args)
   {
     int sock = (int) args;

     //recv request 
     //parse request  解析请求
     //make response  制作响应
     //send response  发送响应
    Connect *conn = new Connect(sock); 
    HttpRequest *rq = new HttpRequest();
    HttpResponse * rsp = new HttpResponse();
    conn->RecvHttpRequest(rq);


    delete conn;
    delete rq;
    delete rsp; 
   }

};

class HttpRequest{
  private:
  std::string request_line; //行
  std::string request_header;  //报头
  std::string blank;// 空行
  std::string request_text; // 请求正文
};

class HttpResponse
{

  std::string status_line; //行
  std::string response_header;  //报头
  std::string blank;// 空行
  std::string response_text; // 响应正文


};

class Connect  // 收 HttpRequest 发 HttpResponse 异常情况进行相应
{
  private:
    int sock;
  public:
    Connect(int _sock) : sock(_sock)
  {}
void RecvHttpRequest(HttpRequest *rq)
{


}
int RecveLine()
{

}


~Connect()
{

}
};

