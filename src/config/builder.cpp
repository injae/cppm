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
            case "type"_h:
                builder.type = it.second.as<std::string>();
                break;
            case "option"_h:
                builder.option = it.second.as<std::string>();
                break;
            }
        }
        return builder;
    }
}