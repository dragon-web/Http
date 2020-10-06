#pragma once 
#include<iostream>
#include <string>
#include<string.h>
#include<sstream>
#include<unordered_map>
#include"Log.hpp"
#include"Protocol.hpp"
#include"Util.hpp"
#include<sys/types.h>
#include<sys/socket.h>
class HttpRequest{
  private:
    std::string request_line; //行
    std::string request_header;  //报头
    std::string blank;// 空行
    std::string request_body; // 请求正文
  private:
    std::string method;
    std::string url;
    std::string version;
    std::unordered_map<std::string,std::string> header_kv;

    std::string path; //这次请求想访问服务器上哪个资源
    std::string query_string;  //这次请求给服务器哪个资源传递参数

  public:
    HttpRequest():blank("\n"){}
    void SetRequestLine(std::string &line)
    {
      request_line = line;
    }
    void Show()
    {
      cout<<"#####################################"<<endl;
      cout<<"Debug:"<<request_line<<endl; 
      cout<<"Debug:"<<request_header<<endl;
      cout<<"Debug:"<<blank<<endl;
      cout<<"Debug:"<<request_body<<endl; 
      cout<<"Debug method"<<method<<endl;
      cout<<"Debug url"<<url<<endl;
      cout<<"Debug version"<<version<<endl;  
      cout<<"Debug path"<<path<<endl; 
      cout<<"Debug query_string"<<query_string<<endl; 
      cout<<"#####################################"<<endl;
    }

    void SetRequestHeader(std::string &header)
    {
      request_header = header;
    }
    //GET /index.html HTTP /1.0 \n
    void RequestLineParse()
    {  
      std::stringstream ss(request_line);    
      ss >> method >> url >> version;
      std::cout<<"Method = "<<method << "Url = "<<url << "Version = "<<version<<endl;
    }
    void SetRequestBody(std::string &body)
    {
      request_body = body;
    }
    void SetUrlToPath()
    {
      // if path == POST 资源就在Url中保存
      path = url;
    }
   void UrlParse()
   {
      //url->path  query_string 可能不存在
     std::size_t pos = url.find('?');
     if(std::string::npos == pos)
     {
        path = url; // 不带任何参数的url
     }
     else{
      path = url.substr(0,pos);
      query_string = url.substr(pos+1);
     }
   }
    int GetContantLength()
    {
      auto it = header_kv.find("Content-Length");
      if(it == header_kv.end())
      {
        LOG(Warning,"Post method,but not request body");
        return Normal_Error;
      }
        return Util::StringToInt(it->second);
    }
    
    bool IsMethodOK()
    {    
      if(strcasecmp(method.c_str(),"GET") == 0 || strcasecmp(method.c_str(),"POST") == 0)
      {
        return true; 
      }
      return false;
    }
    bool IsGet()
    {
      return strcasecmp(method.c_str(),"GET") == 0;
    }
    //读取http请求正文,放进rq->RequestBody

    void RequestHeaderPause()
    {
      size_t pos = request_header.find('\n');
      int start = 0;
      while (pos != std::string::npos) //找到\n的位置
      {
        std::string sub = request_header.substr(start, pos - start);
        Util::MakeKV(header_kv,sub);
        start = pos + 1;
        pos = request_header.find('\n',pos+1);
      }
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
            line.push_back(c);
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
    
    void RecvHttpBody(HttpRequest *rq)
    {
      int content_Length = rq->GetContantLength();
      if(content_Length > 0)
      {
        std::string body;
        char c;
        while(content_Length > 0)
        {
          recv(sock,&c,1,0);
          body.push_back(c);
          content_Length--;
        }
        rq->SetRequestBody(body);
      }
      rq->SetUrlToPath();
    }
    ~Connect()
    {

    }
};

class Entry
{ 
  public:
    static void *HandlerRequest(void *args)
    {
      int* p  = (int*)args;
      //recv request 
      //parse request  解析请求
      //make response  制作响应
      //send response  发送响应
      int sock = *p;
      delete p;
      Connect *conn = new Connect(sock); 
      HttpRequest *rq = new HttpRequest();
      HttpResponse * rsp = new HttpResponse();

      conn->RecvHttpRequest(rq);
      rq->RequestLineParse();
      if(!rq->IsMethodOK())
      {
        LOG(Warning,"request Method is not Ok!");
      }
      rq->RequestHeaderPause();
      if(!rq->IsGet())
      {
            //request 请求全部读完    
            //1:分析请求资源是否合法
          conn->RecvHttpBody(rq);
      }
    if(rq->IsGet())
    {
      rq->UrlParse();
    }
    rq->Show();
    //2:分析请求路径中是否携带参数
      delete conn;
      delete rq;
      delete rsp; 
    }
};

