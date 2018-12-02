#include "package/cppm_config.h"
#include "util/filesystem.h"
#include <iostream>

namespace cppm
{
    CppmConfig CppmConfig::load() {
        auto home = std::string(std::getenv("HOME"));
        auto config_path = home + "/.cppm/";
        util::create(config_path);
        util::create(config_path + "cmake/");
        util::create(config_path + "local/");
        util::create(config_path + "repo/");
        util::create(config_path + "config.toml");
        parse(cpptoml::parse_file(config_path +"config.toml"));
        return *this;
    }

    void CppmConfig::parse(table_ptr table) {
        package.parse(table);
    }
}
