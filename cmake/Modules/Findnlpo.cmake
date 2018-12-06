include(ExternalProject)

message("pass")
find_package(nlpo)
if(NOT nlpo_FOUND)
ExternalProject_Add(
nlpo
GIT_REPOSITORY https://github.com/INJAE/nlpo.git
GIT_TAG master
SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/nlpo
)
find_package(nlpo)
endif()
