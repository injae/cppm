#ifndef __CPPM_CONFIG_DEPENDENCY_H__
#define __CPPM_CONFIG_DEPENDENCY_H__

#include <nieel/version.h>

namespace cppm
{
    struct Dependency
    {
        std::string name;
        nieel::Version version; 
        std::string cmake_name;
    };
    
    Dependency parse_dependency(std::string&& src);
}

#endif