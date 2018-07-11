#include"config/builder.h"
#include<nieel/util/hash.hpp>

namespace cppm
{
    Builder Builder::parse(YAML::Node& node) {
        using namespace nieel;
        Builder builder;
        for(auto it : node["builder"]) {
            auto property = it.first.as<std::string>().c_str(); 
            switch(hash(property))
            {
            case hash("type"):
                builder.type = it.second.as<std::string>();
                break;
            case hash("cpu-core"):
                builder.cpu_core = it.second.as<std::string>();
                break;
            }
        }
        return builder;
    }
}