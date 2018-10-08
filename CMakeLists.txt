cmake_minimum_required(VERSION 3.6)

project(cppm LANGUAGES CXX VERSION 1.0.3.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(MODULE_PATH ${CMAKE_MODULE_PATH}/Modules)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pthread -fPIC -O0")
include(${CMAKE_MODULE_PATH}/cmake_option.cmake)
include(${CMAKE_MODULE_PATH}/project_maker.cmake)
include(${CMAKE_MODULE_PATH}/utils.cmake)
get_third_party_list()

set(thirdparty 
	${Boost_LIBRARIES}
	nlpo::nlpo
	fmt::fmt
	cpptoml
)
set(source 
	src/option/make.cpp
	src/option/build.cpp
	src/option/cppm.cpp
	src/util/filesystem.cpp
	src/main.cpp
	src/config/config.cpp
	src/config/builder.cpp
	src/config/lib.cpp
	src/config/package.cpp
	src/config/path.cpp
	src/config/bin.cpp
	src/config/dependency.cpp
	src/config/cmake.cpp
	src/config/compiler.cpp
	include/option/make.h
	include/option/build.h
	include/option/cppm.h
	include/option/base_option.h
	include/util/filesystem.h
	include/util/algorithm.hpp
	include/.#config.h
	include/config/dependency.h
	include/config/config.h
	include/config/base_config.h
	include/config/package.h
	include/config/cmake.h
	include/config/path.h
	include/config/lib.h
	include/config/bin.h
	include/config/compiler.h
	include/config/builder.h
)
build_binary(cppm "${source}" "${thirdparty}")



