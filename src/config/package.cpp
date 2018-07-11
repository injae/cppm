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
                case hash("name"):
                    package.name     = it.second.as<std::string>(); 
                    break;
                case hash("type"):
                    package.type     = it.second.as<std::string>(); 
                    break;
                case hash("version"):
                    package.version  = it.second.as<std::string>();
                    break;
                case hash("license"):
                    package.license  = it.second.as<std::string>();  
                    break;
                case hash("authors"):
                    package.authors  = it.second.as<std::string>(); 
                    break;
                case hash("thirdparty-repo"):
                    for(auto repo : node["package"]["thirdparty_repo"])
                        package.thirdparty_repo.push_back(repo.as<std::string>());
                    break;
            }
        }
        return package;
    }
}