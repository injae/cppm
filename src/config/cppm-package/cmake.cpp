#include"config/cppm-package/cmake.h"
#include<nieel/util/hash.hpp>

namespace cppm::package
{
    Cmake Cmake::parse(YAML::Node& node) {
        using namespace nieel;
        Cmake cmake;
        for(auto it : node["cmake"]) {
            auto property = it.first.as<std::string>();
            switch(hash(property))
            {
            case hash("name"):
                cmake.name = it.second.as<std::string>();
                break;
            case hash("include_var_name"):
                cmake.include_var_name = it.second.as<std::string>();
                break;
            case hash("library_var_name"):
                cmake.library_var_name = it.second.as<std::string>();
                break;
            case hash("find-library"):
                for(auto script : node["cmake"]["find-library"])
                    cmake.find_library.push_back(script.as<std::string>());
                break;
            }
        }
        return cmake;
    }
}