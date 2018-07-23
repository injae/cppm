#include "config/dependency.h"
#include <nieel/string.hpp>

namespace cppm
{
    Dependency parse_dependency(std::string&& src) {
        Dependency dep;
        auto part = nieel::str::split(src.c_str(), ' ');
        dep.name       = part[0]; 
        if(part.size() > 0) dep.version    = part[1]; 
        if(part.size() > 1) dep.cmake_name = part[2]; 
        return dep; 
    }
}