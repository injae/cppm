
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

macro(find_cppkg)
    set(options HUNTER)
    set(oneValueArg)
    set(multiValueArgs COMPONENTS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    list(GET ARG_UNPARSED_ARGUMENTS 0 name)
    list(GET ARG_UNPARSED_ARGUMENTS 1 version)
    set(version_ ${version})
    if(version STREQUAL "lastest")
      set(version "")
    endif()

    if(ARG_HUNTER) 
        message(STATUS "[cppm] Load ${name} hunter file")
        if(DEFINED ARG_COMPONENTS)
          hunter_add_package(${name} COMPONENTS ${ARG_COMPONENTS})
        else()
          hunter_add_package(${name})
        endif()
    endif()

    set(old_cppkg "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${name}.cmake.in")
    set(new_cppkg "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${name}/${version_}/${name}.cmake.in") 

    if(EXISTS ${new_cppkg})
        configure_file(thirdparty/${name}/${version_}/${name}.cmake.in
                       ${CMAKE_BINARY_DIR}/thirdparty/${name}/${version_}/CMakeLists.txt)
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
                                                 "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" .
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/thirdparty/${name}/${version_})
        execute_process(COMMAND cmake  --build .
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/thirdparty/${name}/${version_})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${name}/${version_}/dep.cmake)
            include(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${name}/${version_}/dep.cmake)
        endif()
    else()
        if(EXISTS ${old_cppkg})
            message(STATUS "[cppm] Load ${name} cppkg file")
            configure_file(thirdparty/${name}.cmake.in
                           ${CMAKE_BINARY_DIR}/thirdparty/${name}/CMakeLists.txt)
            execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/thirdparty/${name})
            execute_process(COMMAND cmake  --build .
                            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/thirdparty/${name})
        endif()
    endif()

    if(DEFINED ARG_COMPONENTS)
        find_package(${name} ${version} COMPONENTS ${ARG_COMPONENTS} QUIET)
    else()
        find_package(${name} ${version} QUIET)
    endif()

   if("${${name}_FOUND}")
     message(STATUS "[cppm] Find Package: ${name}/${${name}_VERSION}")
   endif()

endmacro()

macro(cppm_target_define)
    cmake_parse_arguments(ARG "BINARY;STATIC;SHARED;INTERFACE" "" "SOURCES" ${ARGN})
    list(GET ARG_UNPARSED_ARGUMENTS 0 name)

    if(${name} MATCHES ${CMAKE_PROJECT_NAME})
        set(lib_include_dir "include")
        set(lib_source_dir  "src")
    else()
        set(lib_include_dir "include/${CMAKE_PROJECT_NAME}/${name}")
        set(lib_source_dir  "src/${name}")
    endif()

    if(${ARG_BINARY})
        add_executable(${name} "")
        set(${name}_target_type "BINARY")
        target_include_directories(${name}
            PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
    elseif(${ARG_STATIC} OR ${ARG_SHARED})
        set(${name}_target_type "LIBRARY")
        if(${ARG_STATIC})
            add_library(${name} STATIC "")
        elseif(${ARG_SHARED})
            add_library(${name} SHARED "")
        endif()
        add_library(${PROJECT_NAME}::${name} ALIAS ${name})
        set_target_properties(${name} PROPERTIES LINKER_LANGUAGE CXX)
        target_include_directories(${name}
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${lib_include_dir}>
                $<INSTALL_INTERFACE:include>
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/${lib_source_dir}
        )
    elseif(${ARG_INTERFACE})
        set(${name}_target_type "LIBRARY")
        add_library(${name} INTERFACE)
        add_library(${PROJECT_NAME}::${name} ALIAS ${name})
        target_include_directories(${name}
            INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${lib_include_dir}>
                $<INSTALL_INTERFACE:include>
        )
    endif()

    if(DEFINED ARG_SOURCES)
        target_sources(${name} PRIVATE ${ARG_SOURCES})
    endif()
endmacro()

macro(cppm_target_dependencies)
    cmake_parse_arguments(ARG "" "" "PRIVATE;PUBLIC;INTERFACE" ${ARGN})
    list(GET ARG_UNPARSED_ARGUMENTS 0 name)
    if(DEFINED ARG_PUBLIC)
        target_link_libraries(${name} PUBLIC ${ARG_PUBLIC})
    endif()
    if(DEFINED ARG_PRIVATE)
        target_link_libraries(${name} PRIVATE ${ARG_PRIVATE})
    endif()
    if(DEFINED ARG_INTERFACE)
        target_link_libraries(${name} INTERFACE ${ARG_INTERFACE})
    endif()
    set(deps)
    string(REPLACE " " "\n   - " pub_dep "${ARG_PRIVATE}")
    string(REPLACE ";" "\n   - " pub_dep "${ARG_PRIVATE}")
    list(APPEND deps "${pub_dep}")
    string(REPLACE " " "\n   - " pri_dep "${ARG_PUBLIC}")
    string(REPLACE ";" "\n   - " pri_dep "${ARG_PUBLIC}")
    list(APPEND deps "${pri_dep}")
    string(REPLACE " " "\n   - " int_dep "${ARG_INTERFACE}")
    string(REPLACE ";" "\n   - " int_dep "${ARG_INTERFACE}")
    list(APPEND deps "${int_dep}")
    string(REPLACE ";" " " deps "${deps}")
    message(STATUS "Dependencies:")
    message("   - ${deps}\n")
endmacro()

function(cppm_target_install)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})
    list(GET ARG_UNPARSED_ARGUMENTS 0 name)
    if(${${name}_target_type} MATCHES "BINARY")
        install(TARGETS ${name} RUNTIME DESTINATION bin)
    endif()
    if(${${name}_target_type} MATCHES "LIBRARY")
        install(TARGETS ${name} EXPORT ${PROJECT_NAME}-targets
            ARCHIVE DESTINATION lib 
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin
        )
        install(DIRECTORY include/ DESTINATION include)

        install(EXPORT ${CMAKE_PROJECT_NAME}-targets
            FILE ${CMAKE_PROJECT_NAME}-config.cmake
            NAMESPACE ${CMAKE_PROJECT_NAME}::
            DESTINATION lib/cmake/${CMAKE_PROJECT_NAME}
        )
        # project-config-version install part
        include(CMakePackageConfigHelpers)
        write_basic_package_version_file(
          ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config-version.cmake
          VERSION ${${CMAKE_PROJECT_NAME}_VERSION}
          COMPATIBILITY ExactVersion
        ) 

        install(FILES
          ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config-version.cmake
          DESTINATION lib/cmake/${CMAKE_PROJECT_NAME}
        )
        message(STATUS "Module : ${CMAKE_PROJECT_NAME}::${name}")
        #message(STATUS "cppm.toml: ${CMAKE_PROJECT_NAME} = {module=\"${CMAKE_PROJECT_NAME}::${name}\" version=\"${${CMAKE_PROJECT_NAME}_VERSION}\"}")
    endif()
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

function(git_clone)
endfunction()

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
endfunction()
