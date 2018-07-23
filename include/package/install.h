#ifndef __CPPM_PACKAGE_INSTALL_H__
#define __CPPM_PACKAGE_INSTALL_H__

#include<yaml-cpp/yaml.h>
#include<string>
#include<vector>

namespace cppm::package
{
    class Install
    {
    public:
        static Install parse(YAML::Node& node);
        void install();
    public:
        std::vector<std::string> linux;
        std::vector<std::string> windows;
    };
    
}


#endif
