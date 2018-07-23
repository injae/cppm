#include"package/cmake.h"
#include"cppm.h"
#include<nieel/util/hash.hpp>
#include<nieel/string.hpp>
#include<nieel/filesystem.h>
#include<nieel/algorithm.hpp>
#include<fstream>

namespace cppm::package
{
    
    Cmake Cmake::parse(YAML::Node& node) {
        using namespace nieel;
        Cmake cmake;
        for(auto it : node["package"]["cmake"]) {
            auto property = it.first.as<std::string>();
            switch(hash(property))
            {
            case "name"_h:
                cmake.name = it.second.as<std::string>();
                break;
            case "include_var_name"_h:
                cmake.include_var_name = it.second.as<std::string>();
                break;
            case "library_var_name"_h:
                cmake.library_var_name = it.second.as<std::string>();
                break;
            case "find-library"_h:
                cmake.find_library = it.second.as<std::string>();
                break;
            }
        }
        return cmake;
    }
}