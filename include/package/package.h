#ifndef __CPPM_PACKAGE_PACKAGE_H__
#define __CPPM_PACKAGE_PACKAGE_H__

#include<yaml-cpp/yaml.h>
#include<string>
#include<nieel/filesystem.h>
#include"package/cmake.h"
#include"package/build.h"
#include"package/install.h"
#include<nieel/version.h>

namespace cppm::package
{
    class Package
    {
    public:
        static Package parse(fs::path& path);
        static fs::path find_package(std::string name);
        void install_dependencies();
        void install_pkg();
    public: 
         std::string name;
         std::string repo;
         nieel::Version version;
         std::string lisence;
         std::string authors;
         std::string description;
         std::vector<std::string> dependencies;
         Build build;
         Cmake cmake;
         Install install;
    };
}



#endif