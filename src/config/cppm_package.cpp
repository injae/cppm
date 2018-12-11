#include "config/cppm_package.h"
#include "cppm_version.h"
#include "util/version.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include <iostream>

namespace cppm
{
    void CppmPackage::parse(table_ptr table) {
        if(auto config = table->get_table("config")) {
            auto source = config->get_array_of<std::string>("toolchains");
            if(source) for(const auto& src : *source) {tool_chains.push_back(src);} 
        }
    }
    std::string CppmPackage::toolchains() {
        return util::accumulate(tool_chains, " ").erase(0,1);
    }

    void CppmPackage::add_toolchain(const std::string& toolchain) {
        auto home = std::string(std::getenv("HOME"));
        auto config_path = home +"/.cppm/config.toml";
        util::create(config_path);
        auto table = cpptoml::parse_file(config_path);
        auto config = table->get_table("config");
        if(!config) config = cpptoml::make_table();
        auto source = config->get_array("toolchains");
        if(!source) source = cpptoml::make_array();
        source->push_back(toolchain);
        config->insert("toolchains", source);
        table->insert("config", config);
        std::fstream file(config_path);
        file << *table;
        file.close();
    }
}
