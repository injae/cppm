#include "config/lib.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include "config/config.h"
#include "util/cmake.h"
#include <string>
#include <fmt/format.h>

namespace cppm
{
    void Libs::parse(table_ptr table) {
        auto table_array = table->get_table_array("lib");
        if(!table_array) return;
        for(const auto& lib_table : *table_array) {
            Lib lib;
            lib.name = *lib_table->get_as<std::string>("name");
            lib.type = *lib_table->get_as<std::string>("type");
            lib.install = lib_table->get_as<bool>("type").value_or(true);
            auto source = lib_table->get_array_of<std::string>("source");
            if(source) for(const auto& src : *source) {lib.sources.push_back(src);} 
            auto deps = lib_table->get_array_of<std::string>("dependencies");
            if(deps) for(const auto& dep : *deps) {lib.deps.push_back(dep);} 
            list.emplace_back(std::move(lib));
        }
    }

    std::string Libs::generate(Config& config) {
        using namespace fmt::literals;
        using namespace util::cmake;
        auto lib_type = [&](std::string type) -> std::string {
                 if(type == "shared")           return "SHARED";
           else  if(type == "static")           return "STATIC";
           else  if(type == "header-only")      return "INTERFACE";
           else                                 return "ERROR";
        };
        std::string gen ="";
        for(const auto& lib : list) {
            std::vector<std::string> sources;
            for(const auto& src : lib.sources) {
                auto result = util::find_files(config.path.root, std::regex(src), false);
                sources.insert(sources.end(), result.begin(), result.end());
            }
            std::string sources_;
            if(lib_type(lib.type) != "INTERFACE") {
                 sources_ = "\nSOURCES {}\n"_format(util::accumulate(sources, "\n\t"));
            }
            gen += "\n\ncppm_target_install({0} {1} {2} {3})\n"_format(
                   lib.name, lib_type(lib.type), lib.install ? "INSTALL" : "", sources_);
        }
        return gen;
    }
}
