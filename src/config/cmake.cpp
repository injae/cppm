#include "config/cmake.h"
#include <nieel/util/hash.hpp>

namespace cppm
{
    Cmake Cmake::parse(YAML::Node& node) {
        using namespace nieel;
        Cmake cmake;
        for(auto it : node["cmake"]) {
            auto property = it.first.as<std::string>().c_str();
            switch(hash(property))
            {
                case "version"_h:
                    cmake.version     = it.second.as<std::string>(); 
                    break;
                case "options"_h:
                    cmake.options     = it.second.as<std::string>();
                    break;
            }
        }
        return cmake;
    }
}
