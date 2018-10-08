#include "config/config.h"
#include "config/path.h"
#include "config/package.h"
#include "config/builder.h"
#include "config/bin.h"
#include "config/lib.h"
#include "config/cmake.h"
#include "config/compiler.h"
#include "config/dependency.h"
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
             + "endif()\n"
             + "if(${CMAKE_SYSTEM_NAME} STREQUAL \"Darwin\")\n"
             + "    add_definitions(-DSYSTEM_DARWIN)\n"
             + "endif()\n"
             + "if(${CMAKE_SYSTEM_NAME} STREQUAL \"AIX\")\n"
             + "    add_definitions(-DSYSTEM_AIX)\n"
             + "endif()"
             + "if(${CMAKE_SYSTEM_NAME} MATCHES \"Windows\")\n"
             + "    add_definitions(-DSYSTEM_WINDOWS)\n"
             + "endif()\n"
             + "\n"
             + "# detect host processor\n"
             + "message(STATUS \"The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}\")\n"
             + "set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})\n"
             + "set(MODULE_PATH ${CMAKE_MODULE_PATH}/Modules)\n"
             + "\n"
             + compiler.generate()
             + "\n"
             + dependencies.gen_find_package()
             + "\n"
             + libs.generate(*this)
             + "\n"
             + bins.generate(*this)
             + "\n"
             ;
    }
    
}
