#include"config/cppm-package/install.h"
#include<nieel/util/hash.hpp>

namespace cppm::package
{
   Install Install::parse(YAML::Node& node) {
        using namespace nieel;
        Install install;
        for(auto it : node["install"]) {
            auto property = it.first.as<std::string>();
            switch(hash(property))
            {
            case "linux"_h:
                for(auto script : node["install"]["linux"])
                    install.linux.push_back(script.as<std::string>());
                break;
            case "window"_h:
                for(auto script : node["install"]["window"])
                    install.linux.push_back(script.as<std::string>());
                break;
            }
        }
        return install;
   }
}