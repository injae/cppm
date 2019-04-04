#include "config/dependency.h"
#include <algorithm>
#include <fmt/format.h>
#include <iostream>

using namespace fmt::literals;
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
            list[dependency.name] = dependency;
        } 
    }
    
    std::string Dependencies::gen_find_cppkg() {
        std::string gen;
        for(auto& [name ,dep] : list) {
           auto components = dep.components =="" ? "" : "COMPONENTS " + dep.components;
           auto hunter = dep.hunter ? "HUNTER" : "";
           gen += "find_cppkg({0} {1} {2} {3})\n"_format(
                              name,dep.version,components, hunter);
           //auto module = dep.none_module ? "" :"MODULE {0}"_format(dep.module);
           //auto link_type = dep.link_type;
           //std::transform(link_type.begin(),link_type.end(),link_type.begin(),::toupper);
           //gen += "find_cppkg({0} {1} {2} {3} {4} {5})\n"_format(
           //                   name,dep.version,components,module, link_type , hunter);
           //if(dep.none_module) {
           //    gen += "list(APPEND {}_thirdparty {})\n"_format(dep.link_type,dep.module);
           //}
        }
        return gen;
    }

    std::string Dependencies::gen_find_package() {
        std::string gen;
        for(auto& [name, dep] : list) {
           auto components = dep.components =="" ? "" : "components=\"{}\""_format(dep.components);
           auto version = dep.version == "lastest" ? "" : dep.version; 
           gen += "find_package({0} {1} {2})\n"_format(
                               name,version, dep.components);
        }
        return gen;
    }

    std::string Dependencies::generate() {
        std::string gen;
        gen += "[dependencies]\n";
        for(auto& [name ,dep] : list) {
           auto components = dep.components =="" ? "" : ",components=\"{}\""_format(dep.components);
           auto hunter = dep.hunter ? ",hunter=true" : "";
           auto module = dep.none_module ? "" :"module=\"{0}\""_format(dep.module);
           auto no_module = dep.none_module ? ",no_module=true" : "";
           gen += "{0}={{{1}, version=\"{2}\", {3} lnk_type=\"{4}\" {5} {6}}}\n"_format(
                  name,module,dep.version,components,dep.link_type,hunter,no_module);}
        return gen;
    }

    std::string Dependencies::use_hunter(Hunter& hunter) {
        auto result = std::find_if(list.begin(), list.end(),
                                   [](auto h){ return h.second.hunter == true; });
        if(result == list.begin()) { return hunter.generate(); }
        else { return ""; }
    }
}
