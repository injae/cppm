#ifndef __CPPM_THIRDPARTY_H__
#define __CPPM_THIRDPARTY_H__

#include<string>
#include<vector>

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
          std::string name
        ; std::string version 
        ; std::string include_dir
        ; std::string library_dir
        ; std::string install_type
        ; std::string config_file
        ; std::string cmake_var_name
        ; std::string license
        ; std::string desciption
        ; std::string build_type
        ; Repository  repo
        ; 
    };
    
    Repository classificate_repo(std::string url);
    
    bool has_find_package(Thirdparty thirdparty);
    void install_thirdparty(Thirdparty& library); 
    void git(std::string& url);
    void svn(std::string& url);
    std::string wget(std::string& url);
    void tar(std::string file);
}

#endif