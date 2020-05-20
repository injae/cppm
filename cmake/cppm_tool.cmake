if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR (NOT CMAKE_SYSTEM_NAME AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows"))
    set(env_home "$ENV{USERPROFILE}")
else()
    set(env_home "$ENV{HOME}")
endif()
string(REPLACE "\\" "/" HOME "${env_home}")
set(CPPM_TOOLS_VERSION "0.0.11")

set(CPPM_ROOT   ${HOME}/.cppm)
set(CPPM_CACHE  ${CPPM_ROOT}/cache)
set(CPPM_PKGS   ${CPPM_ROOT}/cppkg)
set(CPPM_CORE   ${CPPM_PKGS}/cppm-tools-${CPPM_TOOLS_VERSION})

if(NOT DEFINED IS_CPPM_LOADED)
set(_install_script "${CPPM_CACHE}/cppm-tools/${CPPM_VERSION}/install-script")
file(WRITE ${_install_script}/CMakeLists.txt
"cmake_minimum_required(VERSION 3.6)
project(CPPM_TOOLS_DOWNLOAD NONE)
include(ExternalProject)
ExternalProject_Add(cppm-tools
    GIT_REPOSITORY https://github.com/injae/cppm_tools.git
    GIT_TAG ${CPPM_TOOLS_VERSION}
    SOURCE_DIR ${CPPM_CORE}
    BINARY_DIR ${CPPM_CACHE}/cppm-tools/${CPPM_TOOLS_VERSION}/build
    CONFIGURE_COMMAND \"\"
    BUILD_COMMAND  \"\"
    INSTALL_COMMAND \"\"
)"
)
execute_process(COMMAND cmake . WORKING_DIRECTORY ${_install_script} OUTPUT_QUIET)
execute_process(COMMAND cmake  --build . WORKING_DIRECTORY ${_install_script} OUTPUT_QUIET)
set(IS_CPPM_LOADED TRUE)
endif()
include(${CPPM_CORE}/core_load.cmake)


