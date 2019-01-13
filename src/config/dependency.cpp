#include "config/dependency.h"
#include <fmt/format.h>
#include <iostream>

namespace cppm
{
    void Dependencies::parse(table_ptr table) {
        auto deps = table->get_table("dependencies");
        if(!deps) return ;
        for(auto& dep_table : *deps) {
            Dependency dependency; 
            dependency.name = dep_table.first;
            auto dep = deps->get_table(dependency.name);
            if(!dep->get_as<std::string>("module")) std::cerr << "need module\n";
            dependency.module = *dep->get_as<std::string>("module");
            dependency.none_module = dep->get_as<bool>("no_module").value_or(false);
            dependency.version     = dep->get_as<std::string>("version").value_or("lastest");
            dependency.components  = dep->get_as<std::string>("components").value_or("");
            list.emplace_back(dependency);
        } 
    }

    void Dependency::insert(table_ptr table) {
        auto deps = table->get_table("dependencies");
        if(!deps) deps = cpptoml::make_table();
        auto dep = deps->get_table(name);
        if(!dep)  dep  = cpptoml::make_table();
        dep->insert("module"    ,module);
        if(version != "")    dep->insert("version"   ,version);
        if(components != "") dep->insert("components", components);
        deps->insert(name, dep);
    }
    
    std::string Dependencies::gen_find_package() {
        using namespace fmt::literals;
        std::string gen;
        for(auto& dep : list) {
           auto components = dep.components =="" ? "" : "COMPONENTS " + dep.components;
           //auto version = dep.version == "lastest" ? "" : dep.version;
           //gen += "download_thirdparty({0} \"{1}\")\n"_format(dep.name, version);
           //gen += "find_package({0} {1} {2} REQUIRED)\n"_format(dep.name, version, components);
           //gen += "list(APPEND thirdparty {0})\n\n"_format(dep.cmake_name);
           if(!dep.none_module) {
               gen += "find_cppkg({0} {1} {2} MODULE {3})\n"_format(dep.name,dep.version,components,dep.module);
           }
           else {
               gen += "find_cppkg({0} {1} {2})\n"_format(dep.name,dep.version,components);
               gen += "list(APPEND thirdparty {})\n"_format(dep.module);
           }
        }
        return gen;
    }
    
}
