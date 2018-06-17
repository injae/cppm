cmake_minimum_required(VERSION 3.6)

project(cppm LANGUAGES CXX VERSION 0.0.1)

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

set(default_project_dir share/${PROJECT_NAME}/default_project)
install(DIRECTORY DESTINATION ${default_project_dir})
install(DIRECTORY DESTINATION ${default_project_dir}/include)
install(DIRECTORY DESTINATION ${default_project_dir}/src)
install(DIRECTORY DESTINATION ${default_project_dir}/thirdparty)
install(DIRECTORY DESTINATION ${default_project_dir}/test)
install(DIRECTORY DESTINATION ${default_project_dir}/bin)
install(DIRECTORY DESTINATION ${default_project_dir}/cmake/Modules)
install(FILES ${CMAKE_MODULE_PATH}/utils.cmake DESTINATION ${default_project_dir}/cmake)
install(FILES ${CMAKE_MODULE_PATH}/cmake_option.cmake DESTINATION ${default_project_dir}/cmake)
install(FILES ${CMAKE_MODULE_PATH}/project_maker.cmake DESTINATION ${default_project_dir}/cmake)

install(DIRECTORY FindThirdparty DESTINATION share/${PROJECT_NAME})

#-ggdb
