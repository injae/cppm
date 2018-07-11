#ifndef __CONFIG_COMPILER_H__
#define __CONFIG_COMPILER_H__

#include<string>
#include<yaml-cpp/yaml.h>

namespace cppm
{
    class Compiler
    {
    public:
        static Compiler parse(YAML::Node& node);
    public:
          std::string type   
        ; std::string option
        ; 
    };
}

#endif