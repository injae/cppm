#include"config/cppm-package/build.h"
#include<nieel/util/hash.hpp>

namespace cppm::package
{
    Build Build::parse(YAML::Node& node) {
        using namespace nieel;
        Build build;
        for(auto it : node["cmake"]) {
            auto property = it.first.as<std::string>();
            switch(hash(property))
            {
            case hash("type"):
                build.type = it.second.as<std::string>();
                break;
            case hash("local"):
                for(auto script : node["build"]["local"])
                    build.local.push_back(script.as<std::string>());
                break;
            case hash("global"):
                for(auto script : node["build"]["global"])
                    build.global.push_back(script.as<std::string>());
                break;
            }
        }
        return build;
    }
}