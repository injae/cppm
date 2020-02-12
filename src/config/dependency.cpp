#include "config/dependency.h"
#include <algorithm>
#include <fmt/format.h>
#include <iostream>
#include <regex>
#include <string>
#include "config/cppm_tool.h"
#include "util/filesystem.h"
#include "config/config.h"

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
                auto dep = util::panic(toml::get_table(deps, dependency.name), "can't find {}"_format(dependency.name));
                dependency.link_type   = toml::get(dep, "lnk_type" , "public");
                dependency.none_module = toml::get(dep, "no_module", false);
                dependency.hunter      = toml::get(dep, "hunter"   , false);
                dependency.type        = toml::get(dep, "type"   , "lib");
                dependency.module      = dependency.hunter ? toml::panic(dep, "module") : toml::get(dep, "module", "");
                dependency.version     = toml::get(dep, "version"   , "latest");
                dependency.components  = toml::get(dep, "components", "");
                dependency.load_path   = toml::get(dep, "load-path", "");
                list[dependency.name] = dependency;
            }
            else {
                auto dep_s = toml::panic(deps, dependency.name);
                std::regex filter("(\\^?)(\\d{1,4}(?:\\.\\d{1,4}){2,3}|latest|git)+(@(stable|nightly|(.*)(?:\\.)(.*)))?");
                std::smatch what;
                if(!std::regex_match(dep_s, what, filter)) { fmt::print("wrong module argument: {}",dep_s); exit(1);}
                dependency.none_module = false;
                dependency.hunter = (what[4] == "hunter") ? true : false;
                if(dependency.hunter) {
                    dependency.module  = what[5];
                    dependency.type    = "lib";
                }
                else if(what[4] == "stable" ||  what[4] == "nightly"){ /* need to make */ }
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
            auto version = dep.version == "latest" ? "" : dep.version; 
            gen += "find_package({0} {1} {2})\n"_format(name, version, dep.components);
        }
        return gen;
    }

    std::string Dependencies::generate() {
        std::string gen;
        for(auto& [name ,dep] : list) {
            if(dep.load_path != "") {
                gen += dep.type == "lib" ? "find_cppkg({0} {1} MODULE {2} LOADPATH)\n"_format(name, dep.version, dep.module) : "";
                gen += "add_subdirectory({})\n"_format(dep.load_path);
                continue;
            }
            auto components = dep.components =="" ? "" : " COMPONENTS " + dep.components;
            auto hunter = dep.hunter ? " HUNTER" : "";
            gen += "find_cppkg({0} {1} MODULE {2}{3}{4})\n"_format(name, dep.version, dep.module, components, hunter);
        }
        return gen;
    }

    std::string Dependencies::use_hunter(Config& config) {
        auto result = std::find_if(list.begin(), list.end(), [](auto h){ return h.second.hunter == true; });
        if(result != list.end()) { config.hunter.use_hunter = true; }
        if(config.hunter.use_hunter) {
            util::over_write_copy_file("{0}cmake/HunterGate.cmake"_format(tool::cppm_root())
                                      ,"{0}/HunterGate.cmake"_format(config.path.cmake));
            return config.hunter.generate();
        }
        else { return ""; }
    }
}
