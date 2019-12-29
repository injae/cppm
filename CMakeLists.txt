cmake_minimum_required(VERSION 3.12)

include(cmake/cppm_tool.cmake)
cppm_project()
include(HunterGate)
HunterGate(
   URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
   SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)
project(cppm VERSION 1.0.9 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)


include(cmake/cppm_install.cmake)
find_cppkg(cpptoml git)
find_cppkg(fmt 6.0.0)
find_cppkg(md5 git)
find_cppkg(nlpo git)
find_cppkg(range-v3 0.9.0)

cppm_target_define(cppm BINARY 
SOURCES 
	src/util/version.cpp
	src/util/filesystem.cpp
	src/util/algorithm.cpp
	src/util/system.cpp
	src/config/cppm_config.cpp
	src/config/compiler.cpp
	src/config/base_target.cpp
	src/config/test.cpp
	src/config/bin.cpp
	src/config/lib.cpp
	src/config/cmake.cpp
	src/config/builder.cpp
	src/config/cppm_package.cpp
	src/config/hunter.cpp
	src/config/dependency.cpp
	src/config/config.cpp
	src/config/cppm_tool.cpp
	src/config/path.cpp
	src/config/package.cpp
	src/option/cppkg_install.cpp
	src/option/cppm.cpp
	src/option/cppkg_search.cpp
	src/option/base_option.cpp
	src/option/cppm_config.cpp
	src/option/cppkg_init.cpp
	src/option/cppkg.cpp
	src/option/init.cpp
	src/option/add.cpp
	src/option/build.cpp
	src/option/install.cpp
	src/main.cpp
	src/package/package.cpp
	include/util/filesystem.h
	include/util/hash.hpp
	include/util/version.h
	include/util/algorithm.hpp
	include/util/system.hpp
	include/util/string.hpp
	include/util/cmake.h
	include/util/command.h
	include/util/;
	include/config/compiler.h
	include/config/dependency.h
	include/config/config.h
	include/config/path.h
	include/config/cppm_config.h
	include/config/base_target.h
	include/config/cppm_package.h
	include/config/test.h
	include/config/hunter.h
	include/config/package.h
	include/config/lib.h
	include/config/cppm_tool.h
	include/config/cmake.h
	include/config/builder.h
	include/config/base_config.h
	include/config/bin.h
	include/option/cppkg_init.h
	include/option/install.h
	include/option/build.h
	include/option/cppm_config.h
	include/option/cppkg_install.h
	include/option/add.h
	include/option/cppkg_search.h
	include/option/base_option.h
	include/option/cppm.h
	include/option/cppkg.h
	include/option/init.h
	include/package/package.h
	include/package/repo.h
	include/cppm_version.h
)


cppm_target_dependencies(cppm 
PUBLIC	
	cpptoml::cpptoml 
	fmt::fmt
	md5::md5 
	nlpo::nlpo 
	range-v3
)

cppm_target_install(cppm)

