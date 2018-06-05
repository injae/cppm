#include"url.h"
#include<regex>
#include<iostream>

Url parse_url(std::string url) {
    std::regex ex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
    std::cmatch what;
    Url parsed_url;
    if(regex_match(url.c_str(), what, ex)) 
    {
        parsed_url.protocol = std::string(what[1].first, what[1].second);
        parsed_url.domain   = std::string(what[2].first, what[2].second);
        parsed_url.port     = std::string(what[3].first, what[3].second);
        parsed_url.path     = std::string(what[4].first, what[4].second);
        parsed_url.query    = std::string(what[5].first, what[5].second);
    }
    
    return parsed_url;
}