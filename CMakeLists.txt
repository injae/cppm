cmake_minimum_required(VERSION 3.6)

project(cppm LANGUAGES CXX VERSION 1.0.0)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_DIR ${PROJECT_ROOT_DIR}/src)
set(INCLUDE_DIR ${PROJECT_ROOT_DIR}/include)
set(THIRD_PARTY_DIR ${PROJECT_ROOT_DIR}/thirdparty)
set(TEST_DIR ${PROJECT_ROOT_DIR}/test)
set(CMAKE_BINARY_DIR ${PROJECT_ROOT_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_ROOT_DIR}/cmake ${CMAKE_MODULE_PATH})
set(MODULE_PATH ${CMAKE_MODULE_PATH}/Modules)
set(BUILD_DIR ${PROJECT_NAME}/build)
set(third_party_library  )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs -g -Wall -pthread -fPIC -O0")
include(${CMAKE_MODULE_PATH}/cmake_option.cmake)
option( BUILD_SHARED_LIB "ON is build shared lib" ON)
include(${CMAKE_MODULE_PATH}/project_maker.cmake)
make_excutable(${PROJECT_NAME})
include(cmake/cppm_install.cmake)


