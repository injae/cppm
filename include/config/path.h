#ifndef __CONFIG_PATH_H__
#define __CONFIG_PATH_H__

#include<string>

namespace cppm
{
    class Path
    {
    public:
        static Path make(const std::string& config_file_path);
    public:
          std::string root 
        ; std::string bin
        ; std::string source
        ; std::string include
        ; std::string thirdparty
        ; std::string cmake_module
        ; std::string cmake_find_module
        ;
    };
    
}

#endif