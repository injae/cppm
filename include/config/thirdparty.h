#ifndef __CPPM_THIRDPARTY_H__
#define __CPPM_THIRDPARTY_H__

#include<string>
#include<vector>
#include<yaml-cpp/yaml.h>
#include"config/repository.h"

namespace cppm
{
    class Thirdparty
    {
    public:
        static Thirdparty parse(const YAML::Node& node, std::string name);
        static std::vector<Thirdparty> parse_thirdparties(const YAML::Node& node);
        
    public:
          std::string name
        ; std::string version 
        ; std::string include_dir
        ; std::string library_dir
        ; std::string install_type
        ; std::string config_file
        ; std::string cmake_var_name
        ; std::string cmake_lib_name
        ; std::string license
        ; std::string desciption
        ; std::string build_type
        ; Repository  repo
        ; 
    };
    
    bool has_find_package(Thirdparty thirdparty);
    void install_thirdparty(Thirdparty& library); 
}

#endif