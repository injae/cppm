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
            dependency.module      = *dep->get_as<std::string>("module");
            dependency.link_type   = dep->get_as<std::string>("lnk_type").value_or("public");
            dependency.none_module = dep->get_as<bool>("no_module").value_or(false);
            dependency.hunter      = dep->get_as<bool>("hunter").value_or(false);
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
           auto hunter = dep.hunter ? "HUNTER" : "";
           auto module = dep.none_module ? "" :"MODULE {0}"_format(dep.module);
           gen += "find_cppkg({0} {1} {2} {3} {4} {5})\n"_format(
                              dep.name,dep.version,components,module, dep.link_type , hunter);
           if(dep.none_module) {
               gen += "list(APPEND {}_thirdparty {})\n"_format(dep.link_type,dep.module);
           }
        }
        return gen;
    }
    
}
