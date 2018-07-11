#ifndef __CONFIG_REPOSITORY_H__
#define __CONFIG_REPOSITORY_H__

#include<string>

namespace cppm
{
    class Repository 
    {
    public:
        static Repository classificate(std::string url);
        
    public:
          std::string url
        ; std::string type
        ; 
    };
    
    void git(std::string& url);
    void svn(std::string& url);
    std::string wget(std::string& url);
    void tar(std::string file);
}
    
#endif