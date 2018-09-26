cmake_minimum_required(VERSION 3.6)

project(cppm LANGUAGES CXX VERSION 1.0.3.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(MODULE_PATH ${CMAKE_MODULE_PATH}/Modules)

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
	src/config/package.cpp
	src/config/path.cpp
	src/config/command.cpp
	src/config/project.cpp
	src/config/dependency.cpp
	src/config/cmake.cpp
	src/config/compiler.cpp
	src/config/thirdparty.cpp
	src/utils.cpp
)
build_binary(cppm "${source}" "${thirdparty}")



