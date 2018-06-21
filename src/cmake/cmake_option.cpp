
#include"cmake/cmake_option.h"
#include<nieel/util/hash.hpp>
#include<iostream>

namespace cmake::option 
{
    std::string build(cppm::Project& project) {
        using namespace nieel;
        switch(hash(project.builder.type))
        {
        case hash("ninja"):
            return " sudo ninja ";
        case hash("make"):
            std::string cmd = " sudo make ";
            if(project.builder.cpu_core != "") cmd += "-j " + project.builder.cpu_core + " ";
            return cmd;
        }
    }
    
    std::string builder(cppm::Project& project) {
        using namespace nieel;
        switch(hash(project.builder.type))
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
    
    std::string compiler(cppm::Project& project) {
        return " -DCMAKE_CXX_COMPILER=" +project.compiler.type;
    }
    
}