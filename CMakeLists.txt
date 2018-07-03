cmake_minimum_required(VERSION 3.6)

project(cppm LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(MODULE_PATH ${CMAKE_MODULE_PATH}/Modules)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs -g -Wall -pthread -fPIC -O0 -Wunused-variable")
include(${CMAKE_MODULE_PATH}/cmake_option.cmake)
include(${CMAKE_MODULE_PATH}/project_makerv2.cmake)
include(${CMAKE_MODULE_PATH}/utils.cmake)
get_third_party_list()

cpp_file_list(src/)
set(thirdparty  Boost::boost yaml-cpp range-v3 nieel)
build_binary(cppm "${source}" "${thirdparty}")

include(cmake/cppm_install.cmake)


