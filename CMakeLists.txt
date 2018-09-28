cmake_minimum_required(VERSION 3.6)

project(cppm LANGUAGES CXX VERSION 1.0.3.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(MODULE_PATH ${CMAKE_MODULE_PATH}/Modules)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs -Wall -pthread -fPIC -O0")
include(${CMAKE_MODULE_PATH}/cmake_option.cmake)
include(${CMAKE_MODULE_PATH}/project_maker.cmake)
include(${CMAKE_MODULE_PATH}/utils.cmake)
get_third_party_list()

set(thirdparty 
	${Boost_LIBRARIES}
	nieel::nieel
	fmt::fmt
	cpptoml
)
set(source 
	src/util/filesystem.cpp
	src/main.cpp
	src/config/config.cpp
	src/config/lib.cpp
	src/config/package.cpp
	src/config/bin.cpp
	src/config/dependency.cpp
)
build_binary(cppm "${source}" "${thirdparty}")



