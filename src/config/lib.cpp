#include "config/lib.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include "config/config.h"
#include "cmake/cmake.h"
#include <string>
#include <fmt/format.h>

namespace cppm
{
    void Libs::parse(table_ptr table) {
        auto table_array = table->get_table_array("lib");
        if(!table_array) return;
        toml::get_table_array(table, "lib", [&](auto tb) {
             Lib lib;
             lib.name    = toml::panic(tb, "name");
             lib.type    = toml::get(tb, "type", "static");
             lib.install = toml::get_bool(tb, "install", true);
             toml::get_array(tb, "source", lib.sources);
             toml::get_array(tb, "dependencies", lib.deps);
             list.emplace_back(std::move(lib));
        });
    }
}
