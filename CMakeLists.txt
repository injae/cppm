cmake_minimum_required(VERSION 3.10)

include(cmake/HunterGate.cmake)
HunterGate(
   URL "https://github.com/ruslo/hunter/archive/v0.23.89.tar.gz"
   SHA1 "a370290a8b32987755a36d64807bf8c7f5b61730"
)
project(cppm LANGUAGES CXX VERSION 1.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
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
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
set(MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -fPIC -O0 -g") 
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -fPIC -O0 -g") 
endif()
find_program(CCACHE_EXE ccache)
if(CCACHE_EXE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    message("-- [cppm] Find ccache")
endif()

list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.cppm/local/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.cppm/local/lib/pkgconfig")
include(${MODULE_PATH}/cppm_tool.cmake)
include(cmake/cppm_install.cmake)
find_cppkg(Boost 1.58.0 COMPONENTS system filesystem MODULE Boost::filesystem HUNTER)
find_cppkg(nlpo lastest  MODULE nlpo::nlpo )
find_cppkg(cpptoml lastest  MODULE cpptoml )
find_cppkg(fmt lastest  MODULE fmt::fmt )


add_executable(cppm "")
target_sources(cppm
	PRIVATE 
	include/cppm_version.h
	include/config/cmake.h
	include/config/lib.h
	include/config/dependency.h
	include/config/path.h
	include/config/builder.h
	include/config/bin.h
	include/config/cppm_package.h
	include/config/config.h
	include/config/base_config.h
	include/config/package.h
	include/config/compiler.h
	include/config/hunter.h
	include/config/cppm_config.h
	include/option/add.h
	include/option/cppkg_init.h
	include/option/build.h
	include/option/cppkg.h
	include/option/install.h
	include/option/init.h
	include/option/base_option.h
	include/option/cppm_config.h
	include/option/cppm.h
	include/util/cmake.h
	include/util/hash.hpp
	include/util/algorithm.hpp
	include/util/system.hpp
	include/util/version.h
	include/util/filesystem.h
	include/util/command.h
	include/package/package.h
	src/config/cmake.cpp
	src/config/package.cpp
	src/config/hunter.cpp
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
	src/option/cppkg_init.cpp
	src/option/cppm_config.cpp
	src/option/install.cpp
	src/option/cppm.cpp
	src/option/cppkg.cpp
	src/option/build.cpp
	src/option/base_option.cpp
	src/util/filesystem.cpp
	src/util/version.cpp
	src/util/algorithm.cpp
	src/main.cpp
	src/package/package.cpp
	src/package/repo.h
)


cppm_target_install(cppm BINARY INSTALL)

