#include "config/dependency.h"
#include <fmt/format.h>

namespace cppm
{
    void Dependencies::parse(table_ptr table) {
        auto deps = table->get_table("dependencies");
        if(!deps) return ;
        for(auto& dep_table : *deps) {
            Dependency dependency; 
            dependency.name = dep_table.first;
            auto dep = deps->get_table(dependency.name);
            dependency.cmake_name       = *dep->get_as<std::string>("module");
            dependency.version          = *dep->get_as<std::string>("version");
            dependency.components = *dep->get_as<std::string>("components");
            list.emplace_back(dependency);
        } 
    }

    void Dependency::insert(table_ptr table) {
        auto deps = table->get_table("dependencies");
        if(!deps) auto deps = cpptoml::make_table();
        auto dep = deps->get_table(name);
        if(!dep) auto dep = cpptoml::make_table();
        dep->insert("module"     ,cmake_name);
        dep->insert("version"   ,version);
        dep->insert("components", components);
        deps->insert(name, dep);
    }
    
    std::string Dependencies::gen_find_package() {
        using namespace fmt::literals;
        std::string gen;
        for(auto& dep : list) {
           auto components = dep.components =="" ? "" : "COMPONENTS " + dep.components;
           gen += "download_thirdparty({0} \"{1}\")\n"_format(dep.name, dep.version);
           gen += "find_package({0} {1} {2} REQUIRED)\n"_format(dep.name, dep.version, components);
           gen += "list(APPEND thirdparty {0})\n\n"_format(dep.cmake_name);
        }
        return gen;
    }
    
}
