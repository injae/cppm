cmake_minimum_required(VERSION 3.12)

set(CPPM_VERSION 0.0.13)
include(cmake/cppm_loader.cmake)
cppm_project()

project(cppm VERSION 0.0.13 LANGUAGES C CXX)
cppm_setting()
cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)

include(cmake/cppm_install.cmake)
find_cppkg(fmt 7.1.3  MODULE fmt::fmt-header-only TYPE lib)
find_cppkg(hashpp git  MODULE hashpp::hashpp TYPE lib)
find_cppkg(nlohmann_json 3.9.1  MODULE nlohmann_json::nlohmann_json TYPE lib)
find_cppkg(nlpo git  MODULE nlpo::nlpo  TYPE lib)
find_cppkg(range-v3 git  MODULE range-v3::range-v3 TYPE lib)
find_cppkg(serdepp 0.0.2  MODULE serdepp::serdepp TYPE lib)
find_cppkg(toml11 3.6.0  MODULE toml11::toml11 TYPE lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
find_cppkg(Catch2 2.9.1  MODULE Catch2::Catch2 TYPE lib)
endif()


cppm_target_define(cppm-core STATIC
SOURCES
    src/core/config.cpp
    src/core/cppm_tool.cpp
    src/util/version.cpp
    src/util/filesystem.cpp
    src/util/algorithm.cpp
    src/util/system.cpp
    src/cppkg/cppkg.cpp
)

cppm_target_define(cppm BINARY
SOURCES
    src/cmake/cmake.cpp
    src/cmake/cmake.h
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
    src/util/version.cpp
    src/util/filesystem.cpp
    src/util/algorithm.cpp
    src/util/system.cpp
    src/main.cpp
)

cppm_examples_area()
if(CPPM_BUILD_EXAMPLES)

cppm_target_define(cppm-core-load BINARY
SOURCES
    examples/example.cpp
)

endif()

cppm_unit_test_area()
if(CPPM_BUILD_TESTING)

cppm_target_define(cppm-test-check BINARY
SOURCES
    tests/test.cpp
)

endif()
end_cppm_unit_test_area()


set(cppm_global_deps PUBLIC fmt hashpp nlohmann_json nlpo range-v3 serdepp toml11)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   list(APPEND cppm_global_deps PUBLIC Catch2)
endif()
cppm_target_dependencies(cppm-core
   ${cppm_global_deps})

cppm_target_dependencies(cppm
   ${cppm_global_deps}
   cppm-core)

cppm_target_dependencies(cppm-core-load
   ${cppm_global_deps}
   cppm-core)

cppm_target_dependencies(cppm-test-check
   ${cppm_global_deps}
   cppm-core)


cppm_target_install(cppm-core)
cppm_target_install(cppm)

