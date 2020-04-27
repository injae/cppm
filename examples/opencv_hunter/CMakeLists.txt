cmake_minimum_required(VERSION 3.6)

include(cmake/cppm_tool.cmake)
cppm_project()
include(HunterGate)
HunterGate(
   URL "https://github.com/ruslo/hunter/archive/v0.23.149.tar.gz"
   SHA1 "c9f170bd697ac6117e68137a2cffdeca679db67e"
)
project(opencv_hunter VERSION 0.0.1 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)


find_cppkg(OpenCV lastest  HUNTER)

cppm_target_define(opencv_hunter BINARY 
SOURCES 
	src/main.cpp
)


cppm_target_dependencies(opencv_hunter 
PRIVATE	
	${OpenCV_LIBS}
)

cppm_target_install(opencv_hunter)

