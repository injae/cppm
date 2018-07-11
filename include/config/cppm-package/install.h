#ifndef __CONFIG_CPPM_PACKAGE_INSTALL_H__
#define __CONFIG_CPPM_PACKAGE_INSTALL_H__

#include<yaml-cpp/yaml.h>
#include<string>
#include<vector>

namespace cppm::package
{
    class Install
    {
    public:
        static Install parse(YAML::Node& node);
    
    public:
        std::vector<std::string> linux;
        std::vector<std::string> window;
    };
    
}


#endif
