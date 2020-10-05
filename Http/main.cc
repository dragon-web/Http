#include"HttpServer.hpp"

static void Usage(std::string proc)
{
  std::cout<<"Usage : "<<proc<<"port [default : 8080]"<<std::endl;
}


int main(int argc ,char* argv[])
{  
  HttpServer *svr = nullptr;
  if(argc == 1)
  {
    LOG(Normal,"create server success ,usage default port!");
    svr = new HttpServer();
  }
  else if (argc == 2)
  {
    svr = new HttpServer(atoi(argv[1]));
    LOG(Normal,"create server success ,usage assign port!");
  }
  else
  {
    Usage(argv[0]);
    exit(ARG_ERR);
  }
  svr->InitServer();
    LOG(Normal,"Init server success!");
 svr->Start();
  std::cout<<"hello Http Server!"<<std::endl;
  return 0;
}
