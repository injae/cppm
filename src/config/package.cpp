#include"config/package.h"
#include<nieel/util/hash.hpp>

namespace cppm
{
    Package Package::parse(YAML::Node& node) {
        using namespace nieel;
        Package package;
        for(auto it : node["package"]) {
            auto property = it.first.as<std::string>().c_str();
            switch(hash(property))
            {
                case "name"_h:
                    package.name     = it.second.as<std::string>(); 
                    break;
                case "type"_h:
                    package.type     = it.second.as<std::string>(); 
                    break;
                case "version"_h:
                    package.version  = it.second.as<std::string>();
                    break;
                case "license"_h:
                    package.license  = it.second.as<std::string>();  
                    break;
                case "authors"_h:
                    package.authors  = it.second.as<std::string>(); 
                    break;
                case "thirdparty-repo"_h:
                    for(auto repo : node["package"]["thirdparty_repo"])
                        package.thirdparty_repo.push_back(repo.as<std::string>());
                    break;
            }
        }
        return package;
    }
}