#include"config/compiler.h"
#include<nieel/util/hash.hpp>

namespace cppm
{
    Compiler Compiler::parse(YAML::Node& node) {
        using namespace nieel;
        Compiler compiler;
        for(auto it : node["compiler"]) {
            auto property = it.first.as<std::string>().c_str(); 
            switch(hash(property))
            {
            case hash("type"):
                compiler.type = it.second.as<std::string>();
                break;
            case hash("option"):
                compiler.option = it.second.as<std::string>();
                break;
            }
        }
        return compiler;
    }
    
}