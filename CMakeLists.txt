cmake_minimum_required(VERSION 3.6)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
include(HunterGate)
HunterGate(
   URL "https://github.com/ruslo/hunter/archive/v0.23.149.tar.gz"
   SHA1 "c9f170bd697ac6117e68137a2cffdeca679db67e"
)
project(cppm LANGUAGES C CXX VERSION 1.0.6)

include(cppm_tool)
cppm_compiler_option(
	DEBUG
		CLANG "-pthread -std=c++17"
		GCC   "-pthread -std=c++17"
	RELEASE
		CLANG "-pthread -std=c++17"
		GCC   "-pthread -std=c++17"
)
cppm_load()


include(cmake/cppm_install.cmake)
find_cppkg(Boost 1.58.0 COMPONENTS system filesystem HUNTER)
find_cppkg(cpptoml lastest  )
find_cppkg(fmt lastest  )
find_cppkg(nlpo lastest  )

cppm_target_define(cppm BINARY 
SOURCES 
	src/util/filesystem.cpp
	src/util/version.cpp
	src/util/algorithm.cpp
	src/config/cmake.cpp
	src/config/path.cpp
	src/config/package.cpp
	src/config/cppm_config.cpp
	src/config/bin.cpp
	src/config/cppm_package.cpp
	src/config/lib.cpp
	src/config/config.cpp
	src/config/builder.cpp
	src/config/hunter.cpp
	src/config/cppm_tool.cpp
	src/config/dependency.cpp
	src/config/base_target.cpp
	src/config/compiler.cpp
	src/main.cpp
	src/option/cppkg.cpp
	src/option/add.cpp
	src/option/cppm_config.cpp
	src/option/build.cpp
	src/option/cppkg_init.cpp
	src/option/base_option.cpp
	src/option/cppm.cpp
	src/option/init.cpp
	src/option/install.cpp
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

