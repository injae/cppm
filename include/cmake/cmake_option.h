
#ifndef __CPPM__CMAKE_CMAKE_OPTION_H__
#define __CPPM__CMAKE_CMAKE_OPTION_H__

#include"cppm.h"
#include<string>

namespace cmake::option 
{
    std::string builder(Cppm::Project& project);
    std::string compiler(Cppm::Project& project);
    std::string build(Cppm::Project& project);
}

#endif  