cmake_minimum_required(VERSION 3.12)

if(NOT IS_CPPM_LOADED)
    include(cmake/cppm_tool.cmake)
endif()
cppm_project()

project(cppm VERSION 0.0.10 LANGUAGES C CXX)
cppm_setting()
cppm_cxx_standard(17)
cppm_compiler_option(
   DEBUG
      MSVC "/std:c++lastest"

   RELEASE
      MSVC "/std:c++lastest"
)

include(cmake/cppm_install.cmake)
find_cppkg(fmt 6.2.0 MODULE fmt::fmt)
find_cppkg(hashpp git MODULE hashpp::hashpp)
find_cppkg(nlpo 1.0.3 MODULE nlpo LOADPATH libs/nlpo)
find_cppkg(range-v3 0.10.0 MODULE range-v3::range-v3)
find_cppkg(tomlpp 0.6.0 MODULE tomlpp LOADPATH libs/tomlpp)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
find_cppkg(Catch2 2.9.1 MODULE Catch2::Catch2)
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


set(global_deps PUBLIC fmt hashpp nlpo range-v3 tomlpp)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   list(APPEND global_deps PUBLIC Catch2)
endif()
cppm_target_dependencies(cppm-core
   ${global_deps})

cppm_target_dependencies(cppm
   ${global_deps}
   cppm-core)

cppm_target_dependencies(cppm-core-load
   ${global_deps}
   cppm-core)

cppm_target_dependencies(cppm-test-check
   ${global_deps}
   cppm-core)


cppm_target_install(cppm-core)
cppm_target_install(cppm)

