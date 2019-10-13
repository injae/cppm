string(REPLACE "\\" "/" HOME "$ENV{HOME}")
set(CPPM_VERSION "1.0.9")
set(CPPM_ROOT   ${HOME}/.cppm)
set(CPPM_CORE   ${CPPM_ROOT}/cmake/core)
list(APPEND CMAKE_MODULE_PATH "${CPPM_CORE}")

macro(cppm_project)
    cmake_parse_arguments(_ARG "NIGHTLY" "" "" ${ARGN})
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

    include(FetchContent)
    FetchContent_Populate(cmake-tools-${CPPM_VERSION}
        GIT_REPOSITORY https://github.com/injae/cppm_tools.git
        GIT_TAG        ${CPPM_VERSION}
        SOURCE_DIR     ${CPPM_CORE}/${CPPM_VERSION}
        QUIET
    )
    FetchContent_Populate(${CPPM_VERSION}
        GIT_REPOSITORY https://github.com/injae/cppkg.git
        SOURCE_DIR     ${CPPM_ROOT}/repo
        QUIET
    )
    include(${CPPM_VERSION}/core_load)

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
