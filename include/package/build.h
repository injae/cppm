#ifndef __CPPM_PACKAGE_BUILD_H__
#define __CPPM_PACKAGE_BUILD_H__

#include<yaml-cpp/yaml.h>
#include<string>
#include<vector>

namespace cppm::package
{
    class Build
    {
    public:
        static Build parse(YAML::Node& node);
        void build();
    public:
        std::string type;
        std::vector<std::string> local;
        std::vector<std::string> global;
    };
}


#endif