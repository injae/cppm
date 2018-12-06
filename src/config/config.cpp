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
        builder.parse(table);
        bins.parse(table);
        libs.parse(table);
        compiler.parse(table);
        dependencies.parse(table);
        cppm_config.load();
    }
    
    std::string Config::generate() {
        using namespace fmt::literals;
        return "cmake_minimum_required(VERSION {0})\n"_format("3.10")
             + "\n"
             + "project({0} LANGUAGES CXX VERSION {1})\n"_format(package.name, package.version)
             + "\n"
             + "message(STATUS \"We are on a ${CMAKE_SYSTEM_NAME} system\")\n"
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
             + "message(STATUS \"The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}\")\n"
             + "list(APPEND CMAKE_MODULE_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules\")\n"
             + "set(MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)\n"
            //+ "list(APPEND CMAKE_PREFIX_PATH \"$ENV{HOME}/.cppm/local\")"
             + "\n"
             + compiler.generate()
             + "\n"
             + "include(${MODULE_PATH}/project_maker.cmake)\n"
             + cmake.generate()
             + "set(thirdparty)\n"
             + dependencies.gen_find_package()
             + "\n"
             + libs.generate(*this)
             + "\n"
             + bins.generate(*this)
             + "\n"
             ;
    }
}
