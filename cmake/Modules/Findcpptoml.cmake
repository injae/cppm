include(ExternalProject)

find_package(cpptoml)

if(NOT cpptoml_FOUND)
ExternalProject_Add(
cpptoml
GIT_REPOSITORY https://github.com/skystrife/cpptoml.git
GIT_TAG master
SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cpptoml
)
find_package(cpptoml)

endif()
message("find cpptoml")
