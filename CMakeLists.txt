cmake_minimum_required(VERSION 3.10)

project(cppm LANGUAGES CXX VERSION 2.0.0)

message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_definitions(-DSYSTEM_LINUX)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_definitions(-DSYSTEM_DARWIN)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "AIX")
    add_definitions(-DSYSTEM_AIX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DSYSTEM_WINDOWS)
endif()
# detect host processor
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
set(MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread -fPIC -O0 -g") 
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread -fPIC -O0 -g") 
endif()

include(${MODULE_PATH}/project_maker.cmake)

include(cmake/cppm_install.cmake)
set(thirdparty)
find_package(nlpo   REQUIRED)
list(APPEND thirdparty nlpo::nlpo)
find_package(fmt   REQUIRED)
list(APPEND thirdparty fmt::fmt)
find_package(cpptoml   REQUIRED)
list(APPEND thirdparty cpptoml)
find_package(Boost  COMPONENTS system filesystem REQUIRED)
list(APPEND thirdparty  ${Boost_LIBRARIES})


set(cppm_source 
	include/config/cmake.h
	include/config/lib.h
	include/config/dependency.h
	include/config/path.h
	include/config/builder.h
	include/config/bin.h
	include/config/config.h
	include/config/base_config.h
	include/config/package.h
	include/config/compiler.h
	include/option/build.h
	include/option/init.h
	include/option/make.h
	include/option/base_option.h
	include/option/cppm.h
	include/util/cmake.h
	include/util/algorithm.hpp
	include/util/filesystem.h
	include/util/command.h
	src/config/cmake.cpp
	src/config/package.cpp
	src/config/bin.cpp
	src/config/builder.cpp
	src/config/config.cpp
	src/config/dependency.cpp
	src/config/path.cpp
	src/config/compiler.cpp
	src/config/lib.cpp
	src/option/init.cpp
	src/option/make.cpp
	src/option/cppm.cpp
	src/option/build.cpp
	src/util/filesystem.cpp
	src/util/algorithm.cpp
	src/main.cpp
)

build_binary(cppm "${cppm_source}" "${thirdparty}")
