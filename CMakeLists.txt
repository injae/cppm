cmake_minimum_required(VERSION 3.6)

include(cmake/cppm_tool.cmake)
cppm_project()
include(HunterGate)
HunterGate(
   URL "https://github.com/ruslo/hunter/archive/v0.23.149.tar.gz"
   SHA1 "c9f170bd697ac6117e68137a2cffdeca679db67e"
)
project(cppm VERSION 1.0.7 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)


include(cmake/cppm_install.cmake)
find_cppkg(Boost 1.58.0 COMPONENTS system filesystem HUNTER)
find_cppkg(Catch2 2.9.1  )
find_cppkg(cpptoml 0.4.0  )
find_cppkg(fmt 5.3.0  )
find_cppkg(nlpo lastest  )

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
	src/package/repo.h
	src/main.cpp
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

