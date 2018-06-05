#ifndef __URL_H__
#define __URL_H__

#include<string>

struct Url
{
      std::string protocol 
    ; std::string domain 
    ; std::string port 
    ; std::string path 
    ; std::string query 
    ;  
};

Url parse_url(std::string url);




#endif