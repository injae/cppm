#include"cmake/cmake_option.h"
#include<nieel/util/hash.hpp>
#include<iostream>

namespace cmake::option 
{
    std::string build(cppm::Project& project) {
        using namespace nieel;
        switch(hash(project.builder.type))
        {
        case "make"_h: {
            std::string cmd = " sudo make ";
            if(project.builder.option != "") cmd += project.builder.option + " ";
            return cmd;
        }
        case "ninja"_h:
            return " sudo ninja ";
        default:
            std::cerr << "wrong argument: " << project.builder.type <<std::endl;
            exit(1);
        }
    }
    
    std::string builder(cppm::Project& project) {
        using namespace nieel;
        switch(hash(project.builder.type))
        {
        case "ninja"_h:
            return " -G Ninja";
        case "make"_h:
            return "";
        default:
            std::cerr << "wrong argument: "<< project.builder.type << std::endl;
            exit(1);
        }
    }
    
    std::string compiler(cppm::Project& project) {
        if(project.compiler.type == "") return "";
        return " -D CMAKE_CXX_COMPILER=" +project.compiler.type;
    }
    
    std::string luncher(cppm::Project& project) {
        if(project.compiler.luncher == "") return "";
        return " -D CMAKE_CXX_COMPILER_LAUNCHER=" +project.compiler.luncher;
    }
    
}