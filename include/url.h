#ifndef __URL_H__
#define __URL_H__

#include<string>
#include<boost/optional.hpp>
using boost::optional;
using boost::none;

struct Url
{
      std::string protocol 
    ; std::string domain 
    ; std::string port 
    ; std::string path 
    ; std::string query 
    ;  
};

optional<Url> parse_url(std::string url);




#endif