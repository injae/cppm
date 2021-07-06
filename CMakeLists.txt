cmake_minimum_required(VERSION 3.12)

set(CPPM_VERSION 0.0.13)
include(cmake/cppm_loader.cmake)
cppm_project()

project(cppm VERSION 0.0.13 LANGUAGES C CXX)
cppm_setting()
cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)

include(cmake/cppm_install.cmake)
find_cppkg(hash-lib git  MODULE hash-lib::hash-lib  TYPE lib)
find_cppkg(serdepp git  MODULE serdepp::serdepp TYPE lib)
find_cppkg(nlpo git  MODULE nlpo::nlpo  TYPE lib)
find_cppkg(range-v3 git  MODULE range-v3::range-v3 TYPE lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
find_cppkg(Catch2 2.9.1  MODULE Catch2::Catch2 TYPE lib)
endif()


cppm_target_define(cppm-core INTERFACE)

cppm_target_define(cppm BINARY
SOURCES
    src/core/config.cpp
    src/core/cppm_tool.cpp
    src/cmake/cmake.cpp
    src/util/version.cpp
    src/util/filesystem.cpp
    src/util/algorithm.cpp
    src/util/system.cpp
    src/option/cppkg_install.cpp
    src/option/cppm.cpp
    src/option/cppkg_search.cpp
    src/option/base_option.cpp
    src/option/cppm_config.cpp
    src/option/cppkg_init.cpp
    src/option/cppkg.cpp
    src/option/init.cpp
    src/option/add.cpp
    src/option/build.cpp
    src/option/install.cpp
    src/cppkg/cppkg.cpp
    src/main.cpp
)

cppm_examples_area()
if(CPPM_BUILD_EXAMPLES)

cppm_target_define(cppm-core-load BINARY
SOURCES
    examples/example.cpp
)

endif()


set(cppm_global_deps PRIVATE hash-lib nlpo range-v3
PUBLIC serdepp)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   list(APPEND cppm_global_deps PRIVATE Catch2)
endif()
cppm_target_dependencies(cppm-core
   ${cppm_global_deps})

cppm_target_dependencies(cppm
   ${cppm_global_deps}
   cppm-core)

cppm_target_dependencies(cppm-core-load
   ${cppm_global_deps}
   cppm-core)


cppm_target_install(cppm-core)
cppm_target_install(cppm)
cppm_target_install(cppm-core-load)

