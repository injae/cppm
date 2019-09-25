cmake_minimum_required(VERSION 3.6)

include(cmake/cppm_tool.cmake)
cppm_project()
include(HunterGate)
HunterGate(
   URL "https://github.com/ruslo/hunter/archive/v0.23.149.tar.gz"
   SHA1 "c9f170bd697ac6117e68137a2cffdeca679db67e"
)
project(cppm VERSION 1.0.8 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)


include(cmake/cppm_install.cmake)
find_cppkg(Boost 1.58.0 COMPONENTS system filesystem HUNTER)
find_cppkg(Catch2 2.9.1  )
find_cppkg(cpptoml 0.4.0  )
find_cppkg(fmt 6.0.0  )
find_cppkg(nlpo git  )

cppm_target_define(cppm BINARY 
SOURCES 
	src/config/base_target.cpp
	src/config/cmake.cpp
	src/config/compiler.cpp
	src/config/cppm_config.cpp
	src/config/lib.cpp
	src/config/dependency.cpp
	src/config/config.cpp
	src/config/hunter.cpp
	src/config/test.cpp
	src/config/bin.cpp
	src/config/cppm_tool.cpp
	src/config/package.cpp
	src/config/path.cpp
	src/config/builder.cpp
	src/config/cppm_package.cpp
	src/option/install.cpp
	src/option/cppm_config.cpp
	src/option/init.cpp
	src/option/cppm.cpp
	src/option/cppkg_install.cpp
	src/option/build.cpp
	src/option/cppkg.cpp
	src/option/add.cpp
	src/option/cppkg_init.cpp
	src/option/base_option.cpp
	src/util/system.cpp
	src/util/algorithm.cpp
	src/util/filesystem.cpp
	src/util/version.cpp
	src/package/package.cpp
	src/main.cpp
	include/config/base_config.h
	include/config/package.h
	include/config/config.h
	include/config/hunter.h
	include/config/builder.h
	include/config/dependency.h
	include/config/compiler.h
	include/config/bin.h
	include/config/cppm_package.h
	include/config/test.h
	include/config/cmake.h
	include/config/base_target.h
	include/config/cppm_config.h
	include/config/path.h
	include/config/lib.h
	include/config/cppm_tool.h
	include/cppm_version.h
	include/option/install.h
	include/option/cppkg.h
	include/option/cppkg_install.h
	include/option/cppkg_init.h
	include/option/cppm.h
	include/option/init.h
	include/option/add.h
	include/option/cppm_config.h
	include/option/base_option.h
	include/option/build.h
	include/util/.#system.hpp
	include/util/algorithm.hpp
	include/util/hash.hpp
	include/util/string.hpp
	include/util/command.h
	include/util/filesystem.h
	include/util/cmake.h
	include/util/system.hpp
	include/util/version.h
	include/package/package.h
	include/package/repo.h
)


cppm_target_dependencies(cppm 
PUBLIC	
	Boost::filesystem
	Catch2::Catch2
	cpptoml
	fmt::fmt
	nlpo::nlpo 
)

cppm_target_install(cppm)

