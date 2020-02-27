#include "config/cppm_tool.h"
#include <vector>
#include <regex>
#include <fmt/format.h>
#include "util/algorithm.hpp"
#include "util/filesystem.h"
#include "util/system.hpp"
#include <string.h>

using namespace fmt::literals;
using namespace std::literals;

namespace cppm::tool
{
    std::string cppm_root() {
        return "{}.cppm/"_format(util::system::home_path());
    }

    std::string target_define(Config::ptr config) {
        std::string gen;
        for(auto& lib : config->libs.list) {
            auto sources = lib.get_sources(config);
            std::string type = lib.type;
            std::string gen_sources = "";
            if(lib.type == "header-only") { type = "INTERFACE"; }
            else {
                std::transform(type.begin(), type.end(), type.begin(), ::toupper);
                gen_sources = "\nSOURCES {0}"_format(util::accumulate(sources, "\n\t"));
            }
            gen += "\ncppm_target_define({0} {1} {2})\n"_format(lib.name, type, gen_sources);
        }

        for(auto& bin : config->bins.list) {
            auto sources = bin.get_sources(config);
            gen += "\ncppm_target_define({0} {1} \nSOURCES {2}\n)\n"_format(
                    bin.name, "BINARY", util::accumulate(sources, "\n\t"));
        }

        return gen;
    }

    std::string target_dependencies(Config::ptr config) {
        std::string gen;

        std::vector<base_target*> list;
        for(auto& lib : config->libs.list) { list.emplace_back(&lib); }
        for(auto& bin : config->bins.list) { list.emplace_back(&bin); }
        //if(config.test.is_testable) { list.emplace_back(&config.test); }

        for(auto target : list) {
            if(target->deps.empty()) {
                for(auto& g_dep : config->dependencies.list) {
                    if(g_dep.second.type == "lib") target->deps.push_back(g_dep.first);
                }
            }
            std::vector<std::string> pub_deps, pri_deps, int_deps;
            for(const auto& dep : target->deps) {
                auto g_dep = config->dependencies.list[dep];
                     if(g_dep.link_type == "public")    pub_deps.push_back(g_dep.name);
                else if(g_dep.link_type == "private")   pri_deps.push_back(g_dep.name);
                else if(g_dep.link_type == "interface") int_deps.push_back(g_dep.name);
            }
            auto trans = [&](std::string title, std::vector<std::string> list){
                             return "\n{0}\t{1}"_format(title,util::accumulate(list, "\n\t"));
                         };
            std::string gen_dep; 
            if(!pub_deps.empty()) gen_dep += trans("PUBLIC"   , pub_deps);
            if(!pri_deps.empty()) gen_dep += trans("PRIVATE"  , pri_deps);
            if(!int_deps.empty()) gen_dep += trans("INTERFACE", int_deps);
            gen += "\n\ncppm_target_dependencies({0} {1}\n)\n"_format(target->name, gen_dep);
        }
        return gen;
    }

    std::string target_install(Config::ptr config) {
        std::string gen;

        std::vector<base_target*> list;
        for(auto& lib : config->libs.list) { list.push_back(&lib); }
        for(auto& bin : config->bins.list) { list.push_back(&bin); }

        for(auto target : list) {
            if(target->install) gen += "\ncppm_target_install({})\n"_format(target->name);
        }
        return gen;
    }

}
