#ifndef __CPPM_THIRDPARTY_H__
#define __CPPM_THIRDPARTY_H__

#include<string>

namespace cppm
{
    
    struct Repository 
    {
          std::string url
        ; std::string type
        ;
    };
    
    struct Thirdparty
    {
          std::string     name
        ; std::string     version 
        ; std::string     license
        ; std::string     desciption
        ; std::string     build_type
        ; Repository repo
        ; 
    };
    
    Repository classificate_repo(std::string url);
   
    void install_thirdparty(Thirdparty library); 
    void git_clone();
    void wget();
    void tar();
}

#endif