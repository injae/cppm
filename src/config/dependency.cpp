#include "config/dependency.h"
#include <algorithm>
#include <fmt/format.h>
#include <iostream>
#include <regex>
#include <string>
#include "config/cppm_tool.h"
#include "util/filesystem.h"
#include "config/config.h"
#include "util/version.h"
#include "cppkg/cppkg.h"

using namespace fmt::literals;
namespace cppm
{
    void Dependency::parse(table_ptr table) {
          version      = toml::get(table, "version"  , "latest");
          type         = toml::get(table, "type"     , "lib");
          desc         = toml::get(table, "description", "");
          link_type    = toml::get(table, "link"     , "public");
          none_module  = toml::get_bool(table, "no_module", false);
          hunter       = toml::get_bool(table, "hunter"   , false);
          module       = hunter ? toml::panic(table, "module") : toml::get(table, "module", "");
          flags        = toml::get(table, "flags", "");
          components   = toml::get(table, "components", "");
          load_path    = toml::get(table, "load-path", "");
          helper       = toml::get(table, "helper", "");
          custom       = toml::get(table, "custom", false);
          download.url = toml::get(table, "git", "");
          if(download.url == "") {
              download.url = toml::get(table, "url", "");
              download.is_git = false;
          }
          else {
              download.branch = toml::get(table, "branch", "");
              download.is_git = true;
          }
    }

    void Dependencies::parse(table_ptr table) {
        auto deps = table->get_table("dependencies");
        if(!deps) return ;
        for(auto& dep_table : *deps) {
            Dependency dependency; 
            dependency.name = dep_table.first;
            if(dep_table.second->is_table()) {
                auto dep = util::panic(toml::get_table(deps, dependency.name), "can't find {}"_format(dependency.name));
                dependency.parse(dep);
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
                    dependency.module = what[5];
                    dependency.type   = "lib";
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

    void Dependencies::after_init(Config::ptr config) {
        fs::create_directory(config->path.thirdparty);
        for(auto& [name, dep] : list) {
            if(dep.hunter) continue;
            else if(dep.load_path != "") {
                util::panic(fs::exists("{}/{}"_format(config->path.root, dep.load_path))
                          ,"[cppm-error] can't find load-path package, {}/{}\n"_format(config->path.root,dep.load_path));
                dep.load_path = "{}/{}"_format(config->path.root, dep.load_path);
                continue;
            }
            else {
                auto path = Version::find_version_folder("{}/{}"_format(config->path.thirdparty,name), dep.version);
                if(!path) {
                    cppkg::install(config, cppkg::search(dep.name, dep.version));
                    path = Version::find_version_folder("{}/{}"_format(config->path.thirdparty,name), dep.version);
                }
                auto load_dep  = cppkg::parse(name, *path);
                dep = load_dep;
                auto cmake_file = "{}/{}.cmake.in"_format(*path, dep.name);
                if(!fs::exists(cmake_file)) { util::write(cmake_file ,cppkg::translate(dep)); }
            }
        }
    }

    std::string Dependencies::use_hunter(Config::ptr config) {
        auto result = std::find_if(list.begin(), list.end(), [](auto h){ return h.second.hunter == true; });
        if(result != list.end()) { config->hunter.use_hunter = true; }
        if(config->hunter.use_hunter) {
            util::over_write_copy_file("{0}cmake/HunterGate.cmake"_format(tool::cppm_root())
                                      ,"{0}/HunterGate.cmake"_format(config->path.cmake));
            return config->hunter.generate();
        }
        else { return ""; }
    }
}
