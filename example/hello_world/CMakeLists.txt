cmake_minimum_required(VERSION 3.12)

include(cmake/cppm_tool.cmake)
cppm_project()
project(hello_world VERSION 0.0.1 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)



cppm_target_define(hello_world BINARY 
SOURCES 
	src/main.cpp
)


cppm_target_dependencies(hello_world 
)

cppm_target_install(hello_world)

