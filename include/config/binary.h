#ifndef __CONFIG_BINARY_H__
#define __CONFIG_BINARY_H__

#include<string>
#include<vector>
#include<yaml-cpp/yaml.h>

namespace cppm
{
    class Binary
    {
    public:
        static Binary parse(YAML::Node& node, std::string name);
        static std::vector<Binary> parse_binaries(YAML::Node& node);
        std::string cmake_script();
        
    public:
          std::string name
        ; std::string version
        ; std::vector<std::string> include
        ; std::vector<std::string> source
        ; std::vector<std::string> dependencies
        ;
    };
}

#endif