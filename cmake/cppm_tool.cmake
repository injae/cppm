string(REPLACE "\\" "/" HOME "$ENV{HOME}")
set(CPPM_VERSION "1.0.8")
set(CPPM_ROOT   ${HOME}/.cppm)
set(CPPM_TOOL   ${CPPM_ROOT}/tool)
set(CPPM_MODULE ${CPPM_ROOT}/cmake)
set(CPPM_CORE   ${CPPM_MODULE}/core)
list(APPEND CMAKE_MODULE_PATH "${CPPM_ROOT}")
list(APPEND CMAKE_MODULE_PATH "${CPPM_TOOL}")
list(APPEND CMAKE_MODULE_PATH "${CPPM_MODULE}")
list(APPEND CMAKE_MODULE_PATH "${CPPM_CORE}")

macro(cppm_load)
    cmake_parse_arguments(_ARG "NIGHTLY" "" "" ${ARGN})
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

    if(NOT EXISTS ${CPPM_CORE})
        find_package(Git REQUIRED)
        if(NOT GIT_FOUND)
            message(FATAL_ERROR "git not found!")
        endif()
        #message(STATUS "[cppm] Downloading cppm tool to ${CPPM_ROOT}/tool")
        make_directory("${CPPM_ROOT}")
        make_directory("${CPPM_CORE}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/injae/cmake_git_tool.git git
            WORKING_DIRECTORY ${CPPM_MODULE}
        )

        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone -b ${CPPM_VERSION} --single-branch https://github.com/injae/cppm_tools.git ${CPPM_VERSION}
            WORKING_DIRECTORY ${CPPM_CORE}
        )
    endif()

    include(git/git)
    git_clone(git
        URL https://github.com/injae/cmake_git_tool.git
        PATH ${CPPM_MODULE} QUIET
    )
    git_clone(${CPPM_VERSION}
        URL https://github.com/injae/cppm_tools.git
        PATH ${CPPM_CORE}
        BRANCH ${CPPM_VERSION} QUIET)
    git_clone(cppkg
        URL https://github.com/injae/cppkg.git
        PATH ${CPPM_ROOT}/repo QUIET)
endmacro()

macro(cppm_project)
    cmake_parse_arguments(_ARG "" "" "" ${ARGN})
    cppm_load()
    include(${CPPM_VERSION}/cppm/cppm_project)
    _cppm_project(${_ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_cxx_standard)
    cmake_parse_arguments(_ARG "" "" "" ${ARGN})
    include(${CPPM_VERSION}/cppm/cxx_standard)
    _cppm_cxx_standard(${_ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_compiler_option)
    include(${CPPM_VERSION}/cppm/compiler_option)
    _cppm_compiler_option(${ARGN})
endmacro()

macro(cppm_setting)
  cmake_parse_arguments(_ARG "" "" "" ${ARGN})
  include(${CPPM_VERSION}/cppm/setting)
  _cppm_setting(${_ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(find_cppkg)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(${CPPM_VERSION}/cppkg/find)
    _find_cppkg(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_target_define)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(${CPPM_VERSION}/cppm/target_define)
    _cppm_target_define(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_target_dependencies)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(${CPPM_VERSION}/cppm/target_dependencies)
    _cppm_target_dependencies(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_target_install)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(${CPPM_VERSION}/cppm/target_install)
    _cppm_target_install(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(download_package)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(${CPPM_VERSION}/cppkg/download)
    _download_package(${ARG_UNPARSED_ARGUMENTS})
endmacro()
