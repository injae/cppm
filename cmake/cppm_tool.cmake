
macro(cppm_setting)
  string(REPLACE "\\" "/" HOME "$ENV{HOME}")
  set(CPPM_ROOT ${HOME}/.cppm)
  list(APPEND CMAKE_PREFIX_PATH "${HOME}/.cppm/local/lib/cmake")
  list(APPEND CMAKE_PREFIX_PATH "${HOME}/.cppm/local/lib/pkgconfig")
  list(APPEND CMAKE_MODULE_PATH "${HOME}/.cppm/tool")
  cppm_load()
  if(NOT ${NO_MESSAGE})
    message("Build Project")
    message(STATUS "[cppm] CMake Version: ${CMAKE_VERSION}")
    message(STATUS "[cppm] System Name: ${CMAKE_SYSTEM_NAME}")
    message(STATUS "[cppm] System Version: ${CMAKE_SYSTEM_VERSION}")
    message(STATUS "[cppm] System Processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    message(STATUS "[cppm] Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "[cppm] Generator: ${CMAKE_GENERATOR}")
  endif()

  find_program(CCACHE_EXE ccache)
  if(CCACHE_EXE)
      set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
      if(NOT ${NO_MESSAGE})
          message(STATUS "[cppm] Find ccache")
      endif()
  endif()
  
  set(CPPM_ROOT "${HOME}/.cppm")
  if(NOT ${NO_MESSAGE})
       message(STATUS "[cppm] CPPM_ROOT: ${HOME}/.cppm")
       message(STATUS "[cppm] Compiler Flags:${CMAKE_CXX_FLAGS}")
  endif()

  if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
      add_definitions(-DSYSTEM_LINUX)
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
      add_definitions(-DSYSTEM_DARWIN)
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "AIX")
      add_definitions(-DSYSTEM_AIX)
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      add_definitions(-DSYSTEM_WINDOWS)
  endif()
endmacro()

function(find_cppkg)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppkg/find/1.0.5)
    _find_cppkg(${ARG_UNPARSED_ARGUMENTS})
endfunction()

function(cppm_target_define)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppm/target/define/1.0.5)
    _cppm_target_define(${ARG_UNPARSED_ARGUMENTS})
endfunction()

function(cppm_target_dependencies)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppm/target/dependencies/1.0.5)
    _cppm_target_dependencies(${ARG_UNPARSED_ARGUMENTS})
endfunction()

function(cppm_target_install)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    include(cppm/target/install/1.0.5)
    _cppm_target_install(${ARG_UNPARSED_ARGUMENTS})
endfunction()

macro(download_package)
    set(options LOCAL GLOBAL)
    set(oneValueArgs URL GIT GIT_TAG)
    set(multiValueArgs CMAKE_ARGS W_CONFIGURE W_BUILD W_INSTALL
                                  L_CONFIGURE L_BUILD L_INSTALL)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    list(GET ARG_UNPARSED_ARGUMENTS 0 name)
    list(GET ARG_UNPARSED_ARGUMENTS 1 version)
    list(REMOVE_AT ARG_UNPARSED_ARGUMENTS 0 1)

    set(NO_MESSAGE TRUE)
    cppm_setting()

    if(ARG_LOCAL)
      set(_INSTALL_PREFIX "-DCMAKE_INSTALL_PREFIX=${HOME}/.cppm/local ")
    elseif(ARG_GLOBAL)
      set(_INSTALL_PREFIX "")
    else()
      message(FATAL_ERROR "Need Option LOCAL or GLOBAL")
    endif()

    set(_version ${version})
    if(${version} STREQUAL "lastest")
      set(version "")
    endif()
    
    include(ExternalProject)
    find_package(${name} ${version} QUIET)
    if(NOT "${${name}_FOUND}" AND NOT "${${name}_FIND_VERSION_EXACT}")
        message(STATUS "[cppm] Can not find ${name} package")
        message(STATUS "[cppm] Download ${name} package")
        if(NOT EXISTS ${HOME}/.cppm/install/${name})
            file(MAKE_DIRECTORY ${HOME}/.cppm/install/${name})
        endif()
        if(NOT WIN32)
          ExternalProject_Add(
            ${name}
            URL ${ARG_URL}
            GIT_REPOSITORY ${ARG_GIT}
            GIT_TAG ${ARG_GIT_TAG}
            SOURCE_DIR ${HOME}/.cppm/install/${name}/${_version}
            CMAKE_ARGS ${CMAKE_ARGS} ${_INSTALL_PREFIX} ${ARG_CMAKE_ARGS}
            CONFIGURE_COMMAND ${ARG_L_CONFIGURE}
            BUILD_COMMAND ${ARG_L_BUILD}
            INSTALL_COMMAND ${ARG_L_INSTALL}
            BUILD_IN_SOURCE true
            ${ARG_UNPARSED_ARGUMENTS}
          )
        else(NOT WIN32)
          ExternalProject_Add(
            ${name}
            URL ${ARG_URL}
            GIT_REPOSITORY ${ARG_GIT}
            GIT_TAG ${ARG_GIT_TAG}
            SOURCE_DIR ${HOME}/.cppm/install/${name}/${_version}
            CMAKE_ARGS ${CMAKE_ARGS} ${_INSTALL_PREFIX} ${ARG_CMAKE_ARGS}
            CONFIGURE_COMMAND ${ARG_W_CONFIGURE}
            BUILD_COMMAND ${ARG_W_BUILD}
            INSTALL_COMMAND ${ARG_W_INSTALL}
            BUILD_IN_SOURCE true
            ${ARG_UNPARSED_ARGUMENTS}
          )
        endif(NOT WIN32)
        message(STATUS "[cppm] Cache Direcroty ${HOME}/.cppm/install/${name}/${_version}")
    else()
        message(STATUS "[cppm] Find ${name} package")
    endif()
endmacro()

function(cppm_load)
    if(NOT EXISTS ${CPPM_ROOT}/tool)
        find_package(Git REQUIRED)
        if(NOT GIT_FOUND)
            message(FATAL_ERROR "git not found!")
        endif()
        message(STATUS "[cppm] Downloading cppm tool")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/injae/cppm_tools.git tool --recursive
            WORKING_DIRECTORY ${CPPM_ROOT}
            )
    endif()
    include(download/git)
    git_clone(tool
        URL  https://github.com/injae/cppm_tools.git
        PATH ${CPPM_ROOT}
        QUIET
    )
    git_clone(cppkg
        URL https://github.com/injae/cppkg.git
        PATH ${CPPM_ROOT}/.cppm/repo/
        QUIET
    )
endfunction()
