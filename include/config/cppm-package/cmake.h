#ifndef __CONFIG_CPPM_PACKAGE_CMAKE_H__
#define __CONFIG_CPPM_PACKAGE_CMAKE_H__

#include<yaml-cpp/yaml.h>
#include<string>
#include<vector>

namespace cppm::package
{
    class Cmake 
    {
    public:
        static Cmake parse(YAML::Node& node);
    public:
        std::string name;
        std::string include_var_name;
        std::string library_var_name;
        std::vector<std::string> find_library;
    };
    
}


#endif