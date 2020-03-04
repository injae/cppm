#include "config/bin.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include "config/config.h"
#include <string>
#include "cmake/cmake.h"
#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm
{
    void Bins::parse(table_ptr table) {
        toml::get_table_array(table, "bin", [&](auto tb) {
             Bin bin; 
             bin.name    = toml::panic(tb, "name");
             bin.install = toml::get_bool(tb, "install", true);
             bin.exclude_flag = tb->template get_as<std::string>("optional");
             bin.exclude = toml::get_bool(tb, "exclude", false);
             toml::get_array(tb, "source"      , bin.sources);
             toml::get_array(tb, "dependencies", bin.deps);
             list.emplace_back(std::move(bin));
        });
    }
}
