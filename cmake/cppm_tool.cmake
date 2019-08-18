string(REPLACE "\\" "/" HOME "$ENV{HOME}")
set(CPPM_ROOT ${HOME}/.cppm)
list(APPEND CMAKE_MODULE_PATH "${HOME}/.cppm/tool")

macro(cppm_load)
    cmake_parse_arguments(_ARG "NIGHTLY" "" "" ${ARGN})
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

    if(NOT EXISTS ${CPPM_ROOT}/tool)
        find_package(Git REQUIRED)
        if(NOT GIT_FOUND)
            message(FATAL_ERROR "git not found!")
        endif()
        message(STATUS "[cppm] Downloading cppm tool to ${CPPM_ROOT}/tool")
        make_directory("${CPPM_ROOT}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/injae/cppm_tools.git tool 
            WORKING_DIRECTORY ${CPPM_ROOT}
        )
    endif()

    set(CPPM_TOOL_VERSION "master")
    if(${ARG_NIGHTLY})
        set(CPPM_TOOL_VERSION "nightly")
    endif()

    include(download/git)
    git_clone(tool
        URL https://github.com/injae/cppm_tools.git
        PATH ${CPPM_ROOT}
        BRANCH ${CPPM_TOOL_VERSION} QUIET)
    git_clone(cppkg
        URL https://github.com/injae/cppkg.git
        PATH ${CPPM_ROOT}/repo
        BRANCH ${CPPM_TOOL_VERSION} QUIET)
endmacro()

macro(cppm_project)
    cmake_parse_arguments(_ARG "" "" "" ${ARGN})
    cppm_load()
    include(cppm/cppm_project/1.0.7)
    _cppm_project(${_ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_cxx_standard)
    cmake_parse_arguments(_ARG "" "" "" ${ARGN})
    include(cppm/cxx_standard/1.0.7)
    _cppm_cxx_standard(${_ARG_UNPARSED_ARGUMENTS})
endmacro()


macro(cppm_compiler_option)
    include(cppm/compiler_option/1.0.7)
    _cppm_compiler_option(${ARGN})
endmacro()

macro(cppm_setting)
  cmake_parse_arguments(_ARG "" "" "" ${ARGN})
  include(cppm/setting/1.0.7)
  _cppm_setting(${_ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(find_cppkg)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppkg/find/1.0.7)
    _find_cppkg(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_target_define)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppm/target/define/1.0.5)
    _cppm_target_define(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_target_dependencies)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppm/target/dependencies/1.0.5)
    _cppm_target_dependencies(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_target_install)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppm/target/install/1.0.5)
    _cppm_target_install(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(download_package)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppkg/download/1.0.7)
    _download_package(${ARG_UNPARSED_ARGUMENTS})
endmacro()
