cmake_minimum_required(VERSION 3.12)

set(CPPM_VERSION 0.0.13)
include(cmake/cppm_loader.cmake)
cppm_project()

project(cppm VERSION 0.0.13 LANGUAGES C CXX)
cppm_setting()
cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)

include(cmake/cppm_install.cmake)
find_cppkg(toml11 3.7.0  MODULE toml11::toml11 TYPE lib)
find_cppkg(CLI11 2.2.0  MODULE CLI11::CLI11 TYPE lib)
find_cppkg(hash-lib 0.8.0  MODULE hash-lib::hash-lib  TYPE lib)
find_cppkg(fmt 8.0.1  MODULE fmt::fmt-header-only TYPE lib)
find_cppkg(serdepp git  MODULE serdepp::serdepp TYPE lib)
find_cppkg(range-v3 0.11.0  MODULE range-v3::range-v3 TYPE lib)
find_cppkg(nlohmann_json 3.9.1  MODULE nlohmann_json::nlohmann_json TYPE lib)
find_cppkg(nlpo git  MODULE nlpo::nlpo  TYPE lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR (SERDEPP_BUILD_EXAMPLES OR (SERDEPP_BUILD_TESTING)))
find_cppkg(Catch2 2.9.1  MODULE Catch2::Catch2 TYPE lib)
endif()


cppm_target_define(cppm-core INTERFACE)

cppm_target_define(cppm BINARY
SOURCES
    src/main.cpp
    src/cmake/cmake.cpp
    src/cmake/cmake.h
    src/core/config.cpp
    src/core/cppm_tool.cpp
    src/cppkg/cppkg.cpp
    src/option/cppkg_install.cpp
    src/option/cppm.cpp
    src/option/cppkg_search.cpp
    src/option/base_option.cpp
    src/option/cppkg_init.h
    src/option/cppm_config.cpp
    src/option/install.h
    src/option/build.h
    src/option/cppkg_init.cpp
    src/option/cppkg.cpp
    src/option/cppm_config.h
    src/option/cppkg_install.h
    src/option/add.h
    src/option/cppkg_search.h
    src/option/base_option.h
    src/option/cppm.h
    src/option/cppkg.h
    src/option/init.cpp
    src/option/add.cpp
    src/option/init.h
    src/option/build.cpp
    src/option/install.cpp
    src/cppm_version.h
    src/util/version.cpp
    src/util/filesystem.cpp
    src/util/algorithm.cpp
    src/util/system.cpp
)

cppm_target_define(cppm-new BINARY
SOURCES
    examples/example.cpp
    src/cmake/cmake.cpp
    src/cmake/cmake.h
    src/core/config.cpp
    src/core/cppm_tool.cpp
    src/cppkg/cppkg.cpp
    src/cli/cppm.cpp
    src/cli/cppkg_search.cpp
    src/cli/build.h
    src/cli/cppkg.cpp
    src/cli/cppkg_search.h
    src/cli/cppm.h
    src/cli/cppkg.h
    src/cli/init.cpp
    src/cli/util.hpp
    src/cli/init.h
    src/cli/build.cpp
    src/util/version.cpp
    src/util/filesystem.cpp
    src/util/algorithm.cpp
    src/util/system.cpp
)


set(cppm_global_deps PRIVATE toml11 CLI11 hash-lib fmt range-v3 nlohmann_json nlpo
PUBLIC serdepp)
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR (SERDEPP_BUILD_EXAMPLES OR (SERDEPP_BUILD_TESTING)))
   list(APPEND cppm_global_deps PRIVATE Catch2)
endif()
cppm_target_dependencies(cppm-core
   ${cppm_global_deps})

cppm_target_dependencies(cppm
   ${cppm_global_deps}
   cppm-core)

cppm_target_dependencies(cppm-new
   ${cppm_global_deps}
   cppm-core)


cppm_target_install(cppm-core)
cppm_target_install(cppm)
cppm_target_install(cppm-new)

