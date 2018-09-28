#include "config/dependency.h"
#include <iostream>

namespace cppm
{
    void Dependencies::parse(table_ptr table) {
        auto deps = table->get_table("dependencies");
        for(auto& dep_table : *deps) {
            Dependency dependency; 
            dependency.name = dep_table.first;
            auto dep = deps->get_table(dependency.name);
            dependency.cmake_name = *dep->get_as<std::string>("cmake");
            dependency.version    = *dep->get_as<std::string>("version");

            list.emplace_back(dependency);
        } 
    }
    
    std::string Dependencies::generate() {
        std::string gen;

        return gen;
    }
}
