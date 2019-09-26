#include "config/cppm_config.h"
#include "config/cppm_tool.h"
#include "util/filesystem.h"
#include <iostream>

namespace cppm
{
    CppmConfig CppmConfig::load() {
        root_path = tool::cppm_root();
        util::create(root_path);
        util::create(root_path + "cmake/");
        util::create(root_path + "local/");
        util::create(root_path + "repo/");
        util::create(root_path + "config.toml");
        parse(cpptoml::parse_file(root_path +"config.toml"));
        return *this;
    }

    void CppmConfig::parse(table_ptr table) {
        package.parse(table);
    }
}
