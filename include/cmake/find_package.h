#ifndef __CMAKE_FIND_PACKAGE_H__
#define __CMAKE_FIND_PACKAGE_H__

#include<vector>
#include<string>
#include"config/thirdparty.h"

namespace cmake 
{
    std::vector<cppm::Thirdparty> find_package_list();
    std::vector<cppm::Thirdparty> package_config_list();
    std::vector<cppm::Thirdparty> get_find_package_list();
    
    bool has_find_package(cppm::Thirdparty thirdparty);
    void make_find_library(cppm::Thirdparty& library);
    void get_package_config_hint(cppm::Thirdparty& thirdparty);
}

#endif