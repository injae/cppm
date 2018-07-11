#ifndef __CONFIG_CPPM_PACKAGE_BUILD_H__
#define __CONFIG_CPPM_PACKAGE_BUILD_H__

#include<yaml-cpp/yaml.h>
#include<string>
#include<vector>

namespace cppm::package
{
    class Build
    {
    public:
        static Build parse(YAML::Node& node);
    public:
        std::string type;
        std::vector<std::string> local;
        std::vector<std::string> global;
    };
}


#endif