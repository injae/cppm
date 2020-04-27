cmake_minimum_required(VERSION 3.12)

if(NOT IS_CPPM_LOADED)
include(cmake/cppm_tool.cmake)
endif()
cppm_project()
project(cmake-tool VERSION 0.0.1 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)


find_cppkg(fmt 6.0.0 MODULE fmt::fmt)

cppm_target_define(cmake-tool STATIC  
SOURCES 
	src/cache.cpp)


cppm_target_dependencies(cmake-tool 
PUBLIC	
	fmt
)

cppm_target_install(cmake-tool)

