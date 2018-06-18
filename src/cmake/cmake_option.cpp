
#include"cmake/cmake_option.h"
#include<nieel/util/hash.hpp>
#include<iostream>

namespace cmake::option 
{
    std::string build(Cppm::Project& project) {
        using namespace nieel::util;
        switch(hash(project.builder.c_str()))
        {
        case hash("ninja"):
            return " sudo ninja ";
            
        case hash("make"):
            std::string cmd = " sudo make ";
            if(project.cpu_core != "") cmd += "-j " + project.cpu_core + " ";
            return cmd;
        }
    }
    
    std::string builder(Cppm::Project& project) {
        using namespace nieel::util;
        switch(hash(project.builder.c_str()))
        {
        case hash("ninja"):
            return " -G Ninja";
        case hash("make"):
            return "";
        default:
            std::cerr << "wrong argument" << std::endl;
            exit(1);
        }
    }
    
    std::string compiler(Cppm::Project& project) {
        return " -DCMAKE_CXX_COMPILER=" +project.compiler;
    }
    
}