#ifndef __CONFIG_CPPM_PACKAGE_PACKAGE_H__
#define __CONFIG_CPPM_PACKAGE_PACKAGE_H__

#include<yaml-cpp/yaml.h>
#include<string>
#include"config/cppm-package/cmake.h"
#include"config/cppm-package/build.h"
#include"config/cppm-package/install.h"
#include<nieel/version.h>

namespace cppm::package
{
    class Package
    {
    public:
        static Package parse(YAML::Node& node);
        static Package find_package(std::string name);
        static void install_package(Package&& package);
    public: 
         std::string name;
         nieel::Version version;
         std::string lisence;
         std::string authors;
         std::string description;
         Build build;
         Cmake cmake;
         Install install;
    };
}



#endif