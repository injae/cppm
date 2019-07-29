#include "config/config.h"
#include "util/cmake.h"
#include "config/cppm_tool.h"
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
        hunter.parse(table);
        builder.parse(table);
        bins.parse(table);
        libs.parse(table);
        compiler.parse(table);
        dependencies.parse(table);
        cppm_config.load();
    }

    void Config::write(table_ptr table) {
        
    }
    
    std::string Config::generate() {
        using namespace fmt::literals;

        return "cmake_minimum_required(VERSION {0})\n"_format(cmake.version)
             + "\n"
             + "include(cmake/cppm_tool.cmake)\n"
             + "cppm_project()\n"
             + dependencies.use_hunter(hunter)
             + "project({0} VERSION {1} LANGUAGES C CXX)\n"_format(package.name, package.version)
             + "cppm_setting()\n"
             + "\n"
             + "cppm_cxx_standard({})\n"_format(package.standard)
             + compiler.generate()
             + "\n"
             + cmake.generate()
             + dependencies.gen_find_cppkg()
             + tool::target_define(*this)
             + tool::target_dependencies(*this)
             + tool::target_install(*this)
             + "\n"
             ;
    }
}
