#ifndef __CONFIG_PACKAGE_H__
#define __CONFIG_PACKAGE_H__

#include<string>
#include<yaml-cpp/yaml.h>
#include<nieel/version.h>

namespace cppm
{
    class Package
    {
    public:
       static Package parse(YAML::Node& node);
    
    public:
          std::string name
        ; std::string type
        ; nieel::Version version
        ; std::string authors
        ; std::string license
        ; std::vector<std::string> thirdparty_repo;
        ;
    };
}

#endif