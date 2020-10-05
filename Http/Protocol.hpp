#pragma once 
#include<iostream>
#include <string>
#include<sstream>
#include<map>
#include"Log.hpp"
#include"Protocol.hpp"
class Connect;
class HttpRequest;
class HttpResponse;
class Entry
{ 
  public:
    bool IsMethodOK()
    {    
      if(strcasecmp(method.c_str(),"GET") == 0 || strcasecmp(method.c_str(),"POST") == 0)
      {
        return true; 
      }
      return false;
    }
    bool ISGet()
    {
      return strcasecmp(method.c_str(),"GET") == 0
    }

    void RequestHeaderPause()
    {
      size_t pos = request_header.find('\n');
      int start = 0;
      while (pos != std::string::npos) //找到\n的位置
      {
        std::string sub = str.substr(start, pos - start);
        MakeKV(header_kv,sub);
        start = pos + 1;
        pos = str.find('\n',pos+1);

      }

    }

    static void *HanderRequest(void *args)
    {
      int sock = (int)args;

      //recv request 
      //parse request  解析请求
      //make response  制作响应
      //send response  发送响应
      Connect *conn = new Connect(sock); 
      HttpRequest *rq = new HttpRequest();
      HttpResponse * rsp = new HttpResponse();

      conn->RecvHttpRequest(rq);
      rq->RequestLinePause();
      if(!rq->IsMethodOk())
      {
        Log(Warning,"request Method is not Ok!");
      }
      rq->RequestHeaderPause();
      if(rq->IsGet())
      {
          
      }


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
  private:
    std::string method;
    std::string url;
    std::string verson;
    std::unorder_map<std::string,std::string> hander_kv;
  public:
    HttpRequest():blank("\n"){}
    void SetRequestLine(std::string &line)
    {
      request_line = line;
    }
    void SetRequestHeader(std::string &header)
    {
      request_header = header;
    }
    //GET /index.html HTTP /1.0 \n
    void RequestLinePause()
    {  
      std::strstream ss(request_line);    
      ss >> method >> url >> verson;
      std::cout<<"Method = "<<method << "Url = "<<url << "Verson = "<<verson<<endl;
    }
    ~HttpRequest()
    {

    }
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

    int RecveLine(std::string &line)
    {
      char c = 'X'; 
      while(c != '\n')
      {
        ssize_t s = recv(sock,&c,1,0);
        if(s > 0)
        {
          if(c == '\r')
          {
            recv(sock,&c,1,MSG_PEEK);//窥探
            if(c == '\n')
            {
              recv(sock,&c,1,0);
            }
            else{
              c = '\n';
            }
          }
          else{
            line.push_bank(c);
          }
        }
        else{
          LOG(Warning,"recv request error!");
          break;
        }
      }
    }
    void RecvHttpRequestLine(std::string &request_line)
    {
      RecveLine(request_line); 
    }

    void RecvHttpRequestHeader(std::string request_header)
    {
      std::string line = "";
      do{
        line = "";
        RecveLine(line);
        if(line != "\n")
        {
          request_header += line;
        }
      }while(line != "\n");

    }


    void RecvHttpRequest(HttpRequest *rq)
    {
      std::string request_line;
      std::string request_header;
      RecvHttpRequestLine(request_line);
      RecvHttpRequestHeader(request_header);

      rq->SetRequestLine(request_line);
      rq->SetRequestHeader(request_header);
    }
    ~Connect()
    {

    }
};

