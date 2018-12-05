include(ExternalProject)

find_package(fmt)

if(NOT fmt_FOUND)
ExternalProject_Add(
fmt
GIT_REPOSITORY https://github.com/fmtlib/fmt.git
GIT_TAG master
SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fmt
)
find_package(fmt)

endif()
