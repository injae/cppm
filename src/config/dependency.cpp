#include "config/dependency.h"
#include <algorithm>
#include <fmt/format.h>
#include <iostream>
#include <regex>
#include <string>

//(.*)+@(\^?)(\d(?:.\d){2,3}|lastest)+(?:\$(stable|nightly|(.*)+(?:\@(.*))))?

using namespace fmt::literals;
namespace cppm
{
    void Dependencies::parse(table_ptr table) {
        auto deps = table->get_table("dependencies");
        if(!deps) return ;
        for(auto& dep_table : *deps) {
            Dependency dependency; 
            dependency.name = dep_table.first;
            if(dep_table.second->is_table()) {
                auto dep = deps->get_table(dependency.name);
                dependency.link_type   = dep->get_as<std::string>("lnk_type").value_or("public");
                dependency.none_module = dep->get_as<bool>("no_module").value_or(false);
                dependency.hunter      = dep->get_as<bool>("hunter").value_or(false);
                if(!dep->get_as<std::string>("module") && dependency.hunter) std::cerr << "need module\n";
                dependency.module      = *dep->get_as<std::string>("module");
                dependency.version     = dep->get_as<std::string>("version").value_or("lastest");
                dependency.components  = dep->get_as<std::string>("components").value_or("");
                dependency.load_path   = dep->get_as<std::string>("load-path").value_or("");
                list[dependency.name] = dependency;
            }
            else {
            //else if(dep_table.second->is_value()){
                auto dep_s = *deps->get_as<std::string>(dependency.name);
                std::regex filter("(\\^?)(\\d{1,4}(?:\\.\\d{1,4}){2,3}|lastest)+(@(stable|nightly|(.*)(?:\\.)(.*)))?");
                std::smatch what;
                if(!std::regex_match(dep_s, what, filter)) { fmt::print("wrong module argument: {}",dep_s); exit(1);}
                dependency.none_module = false;
                dependency.hunter = (what[4] == "hunter") ? true : false;
                if(dependency.hunter) { dependency.module  = what[5]; }
                else if(what[4] == "stable" ||  what[4] == "nightly"){
                    
                }
                if(what[1] == "^") { } // version manager
                dependency.version = what[2];
                dependency.link_type = "public";
                dependency.components = "";
                dependency.load_path = "";
                list[dependency.name] = dependency;
            }
        } 
    }

    std::string Dependencies::gen_find_package() {
        std::string gen;
        for(auto& [name, dep] : list) {
            if(dep.load_path != "") {
                gen += "add_subdirectory({})"_format(dep.load_path);
            }
            auto components = dep.components == "" ? "" : "components=\"{}\""_format(dep.components);
            auto version = dep.version == "lastest" ? "" : dep.version; 
            gen += "find_package({0} {1} {2})\n"_format(name, version, dep.components);
        }
        return gen;
    }

    std::string Dependencies::generate() {
        std::string gen;
        for(auto& [name ,dep] : list) {
            if(dep.load_path != "") {
                gen += "add_subdirectory({})\n"_format(dep.load_path);
                continue;
            }
            auto components = dep.components =="" ? "" : "COMPONENTS " + dep.components;
            auto hunter = dep.hunter ? "HUNTER" : "";
            gen += "find_cppkg({0} {1} {2} {3})\n"_format(name,dep.version,components, hunter);
        }
        return gen;
    }

    std::string Dependencies::use_hunter(Hunter& hunter) {
        auto result = std::find_if(list.begin(), list.end(), [](auto h){ return h.second.hunter == true; });
        if(result == list.begin()) { return hunter.generate(); }
        else { return ""; }
    }
}
