#include"config/path.h"
#include<iostream>

namespace cppm
{
    Path Path::make(const std::string& config_file_path) {
        Path path;
        path.root              = config_file_path; 
        path.bin               = path.root + "/build";
        path.source            = path.root + "/source";
        path.include           = path.root + "/include";
        path.thirdparty        = path.root + "/thirdparty";
        path.cmake_module      = path.root + "/cmake";
        path.cmake_find_module = path.cmake_module + "/Modules";
        return path;
    }
}
