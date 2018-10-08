#include "config/config.h"
#include <fmt/format.h>

namespace cppm
{
    Config Config::load(const std::string &path) {
        auto table = cpptoml::parse_file(path+"/cppm.toml");
        Config config;
        config.path = Path::make(path);
        config.parse(table);
        return config; 
    }

    void Config::parse(table_ptr table) {
        package.parse(table);
        cmake.parse(table);
        builder.parse(table);
        bins.parse(table);
        compiler.parse(table);
        dependencies.parse(table);
    }
    
    std::string Config::generate() {
        using namespace fmt::literals;
        std::string gen
        = "cmake_minimum_required(VERSION {0})\n"_format("3.6")
        + "project({0} LANGUAGES CXX VERSION {1})\n"_format(package.name, package.version)
        + compiler.generate();

        return gen; 
    }
    
}
