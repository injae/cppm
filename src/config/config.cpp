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
             + hunter.generate()
             + "project({0} LANGUAGES CXX VERSION {1})\n"_format(package.name, package.version)
             + "\n"
             + compiler.generate()
             + "include(cmake/cppm_tool.cmake)\n"
             + "cppm_setting()\n"
            //+ "set(CMAKE_CXX_STANDARD {})\n"_format(package.cpp_version)
            //+ "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
            //+ "set(CMAKE_CXX_EXTENSIONS OFF)\n"
             + "# detect host processor\n"
             + "list(APPEND CMAKE_MODULE_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/\")\n"
            //+ "list(APPEND CMAKE_PREFIX_PATH \"$ENV{HOME}/.cppm/local\")"
             + "\n"
             + "\n"
             + cmake.generate()
             + dependencies.gen_find_cppkg()
             + tool::target_define(*this)
             + tool::target_dependencies(*this)
             + tool::target_install(*this)
            //+ "\n"
            //+ libs.generate(*this)
            //+ "\n"
            //+ bins.generate(*this)
             + "\n"
             ;
    }
}
