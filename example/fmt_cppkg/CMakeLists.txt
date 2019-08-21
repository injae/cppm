cmake_minimum_required(VERSION 3.6)

include(cmake/cppm_tool.cmake)
cppm_project()
project(fmt_cppkg VERSION 0.0.1 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)


find_cppkg(fmt 5.3.0  )

cppm_target_define(fmt_cppkg BINARY 
SOURCES 
	src/main.cpp
)


cppm_target_dependencies(fmt_cppkg 
PUBLIC	
	fmt::fmt
)

cppm_target_install(fmt_cppkg)

