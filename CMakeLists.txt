cmake_minimum_required(VERSION 3.10)

project(cppm LANGUAGES CXX VERSION 0.1.0)

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
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.cppm/local")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread -fPIC -O0 -g") 
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread -fPIC -O0 -g") 
endif()
find_program(CCACHE_EXE ccache)
if(CCACHE_EXE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    message("Use ccache")
endif()

<<<<<<< HEAD
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -pthread -fPIC -O0")
include(${CMAKE_MODULE_PATH}/cmake_option.cmake)
include(${CMAKE_MODULE_PATH}/project_maker.cmake)
include(${CMAKE_MODULE_PATH}/utils.cmake)
get_third_party_list()

set(thirdparty 
	yaml-cpp
	${Boost_LIBRARIES}
	nieel::nieel
)
set(source 
	src/url.cpp
	src/package/package.cpp
	src/package/build.cpp
	src/package/install.cpp
	src/package/cmake.cpp
	src/cmake/generator.cpp
	src/cmake/find_package.cpp
	src/cmake/cmake_option.cpp
	src/cppm.cpp
	src/main.cpp
	src/options/init.cpp
	src/options/build.cpp
	src/options/install.cpp
	src/options/gmon.out
	src/cppm_options.cpp
	src/config/library.cpp
	src/config/repository.cpp
	src/config/binary.cpp
	src/config/builder.cpp
=======
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
	include/cppm_version.h
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
	include/util/version.h
	include/util/filesystem.h
	include/util/command.h
	include/package/cppm_package.h
	include/package/cppm_config.h
	src/config/cmake.cpp
>>>>>>> new_version
	src/config/package.cpp
	src/config/bin.cpp
	src/config/builder.cpp
	src/config/config.cpp
	src/config/dependency.cpp
<<<<<<< HEAD
	src/config/cmake.cpp
=======
	src/config/path.cpp
>>>>>>> new_version
	src/config/compiler.cpp
	src/config/lib.cpp
	src/option/init.cpp
	src/option/make.cpp
	src/option/cppm.cpp
	src/option/build.cpp
	src/util/filesystem.cpp
	src/util/version.cpp
	src/util/algorithm.cpp
	src/main.cpp
	src/package/cppm_config.cpp
	src/package/cppm_package.cpp
)

build_binary(cppm "${cppm_source}" "${thirdparty}")
