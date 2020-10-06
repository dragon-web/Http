#pragma once 

#include<iostream>
#include<unordered_map>
#include<string>
#include<sstream>
using namespace std;

class Util
{
  public:
    static void MakeKV(std::unordered_map<std::string,std::string> &header_kv,std::string str)
    {
      size_t pos = str.find(": ");
      if (str.npos == pos)
      {
        return;

      }
      string key = str.substr(0, pos);
      string value = str.substr(pos + 2);
      cout << "key :" << key << endl;
      cout << "value :" << value << endl;
     //unordered_map <string, string> mp;
      //mp.insert(make_pair<string, string>(key, value));
        header_kv.insert({key,value});
    }
  static int StringToInt(std::string &str)
  {
    int len = 0;
    std::stringstream ss(str);
    ss>>len;
    return len; 
  }



};
