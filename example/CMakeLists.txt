cmake_minimum_required(VERSION 3.12)

if(NOT IS_CPPM_LOADED)
include(cmake/cppm_tool.cmake)
endif()
cppm_project()
project(example VERSION 0.0.1 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)


add_subdirectory(fmt_cppkg)
add_subdirectory(hello_world)

