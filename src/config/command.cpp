#include"config/command.h"
#include<nieel/util/hash.hpp>
#include<nieel/algorithm.hpp>
#include<nieel/string.hpp>

namespace cppm
{
    Command Command::parse(YAML::Node& node, std::string name) {
        using namespace nieel;
        Command command;
        command.name = name;
        
        int count = 0;  
        for(auto it : node[name]) {
            count++;
            if(count == 1) {
                command.script = it.as<std::string>();
                continue;
            }
            command.script +=  " && " + it.as<std::string>();
        }
        return command;
    }
}