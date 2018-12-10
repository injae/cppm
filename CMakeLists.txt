cmake_minimum_required(VERSION 3.10)

project(cppm LANGUAGES CXX VERSION 0.1.2)

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
find_program(CCACHE_EXE ccache)
if(CCACHE_EXE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    message("--- use ccache")
endif()

include(${MODULE_PATH}/cppm_tool.cmake)

include(cmake/cppm_install.cmake)
set(thirdparty)
download_thirdparty(nlpo)
find_package(nlpo   REQUIRED)
list(APPEND thirdparty nlpo::nlpo)

download_thirdparty(fmt)
find_package(fmt   REQUIRED)
list(APPEND thirdparty fmt::fmt)

download_thirdparty(cpptoml)
find_package(cpptoml   REQUIRED)
list(APPEND thirdparty cpptoml)

download_thirdparty(Boost)
find_package(Boost  COMPONENTS system filesystem REQUIRED)
list(APPEND thirdparty ${Boost_LIBRARIES})



add_executable(cppm "")
target_sources(cppm
	PRIVATE 
	src/config/cmake.cpp
	src/config/package.cpp
	src/config/bin.cpp
	src/config/builder.cpp
	src/config/cppm_config.cpp
	src/config/cppm_package.cpp
	src/config/config.cpp
	src/config/dependency.cpp
	src/config/path.cpp
	src/config/compiler.cpp
	src/config/lib.cpp
	src/option/add.cpp
	src/option/init.cpp
	src/option/install.cpp
	src/option/make.cpp
	src/option/cppm.cpp
	src/option/build.cpp
	src/util/filesystem.cpp
	src/util/version.cpp
	src/util/algorithm.cpp
	src/main.cpp
	src/package/package.cpp
)
target_link_libraries(cppm PUBLIC ${thirdparty})

target_install(cppm BINARY)

