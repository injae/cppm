cmake_minimum_required(VERSION 3.6)

include(cmake/HunterGate.cmake)
HunterGate(
   URL "https://github.com/ruslo/hunter/archive/v0.23.98.tar.gz"
   SHA1 "bf539cb70375ac319e386ae2ec5b5a0c4923480a"
)
project(cppm LANGUAGES CXX VERSION 1.0.6)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -fPIC -O0 -g -std=c++17") 
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -fPIC -O0 -g -std=c++17") 
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
endif()
include(cmake/cppm_tool.cmake)
cppm_setting()
# detect host processor
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")



include(cmake/cppm_install.cmake)
find_cppkg(Boost 1.58.0 COMPONENTS system filesystem HUNTER)
find_cppkg(cpptoml lastest  )
find_cppkg(fmt lastest  )
find_cppkg(nlpo lastest  )

cppm_target_define(cppm BINARY 
SOURCES 
	src/config/cmake.cpp
	src/config/cppm_tool.cpp
	src/config/package.cpp
	src/config/hunter.cpp
	src/config/bin.cpp
	src/config/builder.cpp
	src/config/cppm_config.cpp
	src/config/cppm_package.cpp
	src/config/config.cpp
	src/config/dependency.cpp
	src/config/path.cpp
	src/config/base_target.cpp
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


cppm_target_dependencies(cppm 
PUBLIC	
	Boost::filesystem
	cpptoml
	fmt::fmt
	nlpo::nlpo
)

cppm_target_install(cppm)

