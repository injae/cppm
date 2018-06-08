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
        ; std::string cmake_var_name
        ; std::string license
        ; std::string desciption
        ; std::string build_type
        ; Repository  repo
        ; 
    };
    
    
    
    Repository classificate_repo(std::string url);
    
    std::vector<std::string> cmake_find_package_list();
    bool has_find_package(Thirdparty thirdparty);
    std::vector<Thirdparty> cmake_package_config_list();
   
    Thirdparty get_package_config_hint(Thirdparty& thirdparty, std::string& file);
   
   
    void make_cmake_find_library(Thirdparty& library);
    void install_thirdparty(Thirdparty& library); 
    void git(std::string& url);
    void svn(std::string& url);
    std::string wget(std::string& url);
    void tar(std::string file);
}

#endif