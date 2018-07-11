#ifndef __CONFIG_COMMAND_H__
#define __CONFIG_COMMAND_H__

#include<string>
#include<vector>
#include<yaml-cpp/yaml.h>

namespace cppm
{
    class Command
    {
    public:
        static Command parse(YAML::Node& node, std::string name);
    public:
          std::string name
        ; std::string script  
        ; 
    };
}

#endif