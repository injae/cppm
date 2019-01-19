#include "config/config.h"
#include "util/cmake.h"
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
        return "cmake_minimum_required(VERSION {0})\n"_format("3.10")
             + "\n"
             + hunter.generate()
             + "project({0} LANGUAGES CXX VERSION {1})\n"_format(package.name, package.version)
             + "\n"
             + "include(cmake/cppm_tool.cmake)\n"
             + "cppm_setting()\n"
             + "set(CMAKE_CXX_STANDARD {})\n"_format(package.cpp_version)
             + "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n"
             + "set(CMAKE_CXX_EXTENSIONS OFF)\n"
             + "if(${CMAKE_SYSTEM_NAME} STREQUAL \"Linux\")\n"
             + "    add_definitions(-DSYSTEM_LINUX)\n"
             + "elseif(${CMAKE_SYSTEM_NAME} STREQUAL \"Darwin\")\n"
             + "    add_definitions(-DSYSTEM_DARWIN)\n"
             + "elseif(${CMAKE_SYSTEM_NAME} STREQUAL \"AIX\")\n"
             + "    add_definitions(-DSYSTEM_AIX)\n"
             + "elseif(${CMAKE_SYSTEM_NAME} MATCHES \"Windows\")\n"
             + "    add_definitions(-DSYSTEM_WINDOWS)\n"
             + "endif()"
             + "\n"
             + "# detect host processor\n"
             + "list(APPEND CMAKE_MODULE_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/\")\n"
            //+ "list(APPEND CMAKE_PREFIX_PATH \"$ENV{HOME}/.cppm/local\")"
             + "\n"
             + compiler.generate()
             + "\n"
             + cmake.generate()
             + dependencies.gen_find_package()
             + "\n"
             + libs.generate(*this)
             + "\n"
             + bins.generate(*this)
             + "\n"
             ;
    }
}
