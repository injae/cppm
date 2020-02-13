#include "config/cmake.h"
#include <fmt/format.h>

namespace cppm
{
    void Cmake::parse(table_ptr table)
    {
        auto cmake = toml::get_table(table, "cmake");
        version  = toml::get(cmake, "version" , "3.12");
        option   = toml::get(cmake, "option"  , "");
        builder  = toml::get(cmake, "builder" , "make");
        compiler = toml::get(cmake, "compiler", "none");
        toml::get_array(cmake, "toolchain", tool_chain);
        toml::get_array(cmake, "include"  , include);
    }

    void Cmake::build_lock(table_ptr table, table_ptr lock) {
        auto origin = toml::get_table(table, "cmake");
        auto cmake = cpptoml::make_table();
        cmake->insert("version" , toml::get(origin, "version" , "3.12"));
        cmake->insert("option"  , toml::get(origin, "option"  , ""));
        cmake->insert("builder" , toml::get(origin, "builder" , "make"));
        cmake->insert("compiler", toml::get(origin, "compiler", "none"));
        lock->insert("cmake", cmake);
    }

    std::string Cmake::generate() {
        using namespace fmt::literals;
        std::string gen = "\n";
        for(auto inc : include) gen += "include({0})\n"_format(inc);
        return gen; 
    }
}
