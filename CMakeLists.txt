cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME cppm)

set(${PROJECT_NAME}_MAJOR_VERISON 1)
set(${PROJECT_NAME}_MINER_VERISON 0)
set(${PROJECT_NAME}_PATCH_VERISON 0)
set(${PROJECT_NAME}_VERSION 
    ${${PROJECT_NAME}_MAJOR_VERION}.${${PROJECT_NAME}_MINER_VERION}.${${PROJECT_NAME}_PATCH_VERION})

project(${PROJECT_NAME})

set(PROJECT_ROOT_DIR       "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_DIR             "${PROJECT_ROOT_DIR}/src")
set(INCLUDE_DIR            "${PROJECT_ROOT_DIR}/include")
set(THIRD_PARTY_DIR        "${PROJECT_ROOT_DIR}/thirdparty")
set(TEST_DIR               "${PROJECT_ROOT_DIR}/test")
set(CMAKE_BINARY_DIR       "${PROJECT_ROOT_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_MODULE_PATH      "${PROJECT_ROOT_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(MODULE_PATH            "${CMAKE_MODULE_PATH}/Modules")
set(BUILD_DIR              "${PROJECT_ROOT_DIR}/build")
set(third_party_library)

include(${CMAKE_MODULE_PATH}/cmake_option.cmake)

include(${CMAKE_MODULE_PATH}/project_maker.cmake)
make_excutable(${PROJECT_NAME})

#add_subdirectory(${TEST_DIR})
enable_testing()

#-ggdb
