#ifndef __CONFIG_CMAKE_H__
#define __CONFIG_CMAKE_H__

#include <string>
#include <yaml-cpp/yaml.h>

namespace cppm
{
    class Cmake
    {
    public:
        static Cmake parse(YAML::Node& node);
    public:
        std::string options;
        std::string version;
    };
}

#endif
