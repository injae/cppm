string(REPLACE "\\" "/" HOME "$ENV{HOME}")
set(CPPM_VERSION "0.0.9")

set(CPPM_ROOT   ${HOME}/.cppm)
set(CPPM_SOURCE ${CPPM_ROOT}/src)
set(CPPM_CACHE  ${CPPM_ROOT}/cache)
set(CPPM_CORE   ${CPPM_SOURCE}/cppm_tools/${CPPM_VERSION})

if(NOT DEFINED IS_CPPM_LOADED)

set(_cache_path "${CPPM_CACHE}/${name}/${VERSION}")
set(_install_script "${CPPM_CACHE}/${name}/${VERSION}/install-script")
file(WRITE ${_install_script}/CMakeLists.txt
"cmake_minimum_required(VERSION 3.6)
project(cpp-tool-${CPPM_VERSION}-install NONE)
include(ExternalProject)
ExternalProject_Add(cppm-tools-${CPPM_VERSION}
    GIT_REPOSITORY https://github.com/injae/cppm_tools.git
    GIT_TAG ${CPPM_VERSION}
    SOURCE_DIR ${CPPM_CORE}
    BINARY_DIR ${_cache_path}/build
    CONFIGURE_COMMAND \"\"
    BUILD_COMMAND  \"\"
    INSTALL_COMMAND \"\"
    #QUIET
)"
)
execute_process(COMMAND cmake . WORKING_DIRECTORY ${_install_script} OUTPUT_QUIET)
execute_process(COMMAND cmake  --build . WORKING_DIRECTORY ${_install_script} OUTPUT_QUIET)
endif()
set(IS_CPPM_LOADED TRUE)
include(${CPPM_CORE}/core_load.cmake)


