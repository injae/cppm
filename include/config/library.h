#ifndef __CONFIG_LIBRARY_H__
#define __CONFIG_LIBRARY_H__

#include<string>
#include<vector>
#include<yaml-cpp/yaml.h>
#include"config/dependency.h"

namespace cppm
{
    class Library
    {
    public:
        static Library parse(YAML::Node& node, std::string name);
        static std::vector<Library> parse_libraries(YAML::Node& node);
        static void show(Library& library);
        std::string cmake_script();
    public: 
          std::string name 
        ; std::string version
        ; std::string type
        ; std::vector<std::string> include
        ; std::vector<std::string> source 
        ; std::vector<Dependency> dependencies
        ;    
    };
}

#endif