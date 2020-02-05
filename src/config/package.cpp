#include "config/package.h"
#include <iostream>
#include <string>
#include <fmt/format.h>

namespace cppm
{
    void Package::parse(table_ptr table) {
        auto package = util::panic(table->get_table("package"), "cppm.toml need package tables");
        name         = toml::panic<std::string>(package, "name");
        version      = toml::get(package, "version", "0.0.1");
        standard     = toml::get(package, "standard", "17"); 
        description  = toml::get(package, "description", "");
        git_repo     = toml::get(package, "git", "");
    }

    void Package::build_lock(table_ptr table, table_ptr lock) {
        auto origin_t = util::panic(table->get_table("package"), "cppm.toml need package tables");
        auto pkg = cpptoml::make_table();
        pkg->insert("name"       , toml::panic<std::string>(origin_t, "name"));
        pkg->insert("version"    , toml::get(origin_t, "version", "0.0.1"));
        pkg->insert("description", toml::get(origin_t, "description", ""));
        pkg->insert("git_repo"   , toml::get(origin_t, "git", ""));
        lock->insert("package", pkg);

        auto global = lock->get_table_qualified("target.default.platform.default");
        global->insert("standard", toml::get(origin_t, "standard", "17"s));
    }
}
