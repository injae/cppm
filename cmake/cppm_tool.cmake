string(REPLACE "\\" "/" HOME "$ENV{HOME}")
set(CPPM_ROOT ${HOME}/.cppm)
list(APPEND CMAKE_MODULE_PATH "${HOME}/.cppm/tool")

macro(cppm_load)
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

    include(download/git)
    git_clone(tool  URL https://github.com/injae/cppm_tools.git PATH ${CPPM_ROOT}       QUIET)
    git_clone(cppkg URL https://github.com/injae/cppkg.git      PATH ${CPPM_ROOT}/repo  QUIET)
    cppm_setting()
endmacro()

macro(cppm_cxx_standard)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppm/cxx_standard/1.0.7)
    _cppm_cxx_standard(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_compiler_option)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppm/compiler_option/1.0.7)
    _cppm_compiler_option(${ARG_UNPARSED_ARGUMENTS})
endmacro()

macro(cppm_setting)
  include(cppm/setting/1.0.5)
  _cppm_setting()
endmacro()

function(find_cppkg)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppkg/find/1.0.5)
    _find_cppkg(${ARG_UNPARSED_ARGUMENTS})
endfunction()

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
    include(cppkg/download/1.0.5)
    _download_package(${ARG_UNPARSED_ARGUMENTS})
endmacro()

