macro(find_cppkg)
    if(NOT DEFINED thirdparty)
      set(thirdparty "")
    endif()
    set(options HUNTER)
    set(oneValueArg)
    set(multiValueArgs COMPONENTS MODULE)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    list(GET ARG_UNPARSED_ARGUMENTS 0 name)
    list(GET ARG_UNPARSED_ARGUMENTS 1 version)
    if(version STREQUAL "lastest")
      set(version "")
    endif()

    if(ARG_HUNTER AND DEFINED ARG_COMPONENTS)
      hunter_add_package(${name} COMPONENTS ${ARG_COMPONENTS})
    endif()
    if(ARG_HUNTER AND NOT DEFINED ARG_COMPONENTS)
      hunter_add_package(${name})
    endif()

    if(DEFINED ARG_COMPONENTS)
      find_package(${name} ${version} COMPONENTS ${ARG_COMPONENTS} QUIET)
    else()
      find_package(${name} ${version} QUIET)
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${name}.cmake.in)
       message("-- [cppm] Load ${name} cppkg file")
       configure_file(thirdparty/${name}.cmake.in ${CMAKE_BINARY_DIR}/thirdparty/${name}/CMakeLists.txt)
       execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/thirdparty/${name})
       execute_process(COMMAND cmake  --build . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/thirdparty/${name} )
   endif()
   if(DEFINED ARG_COMPONENTS)
     find_package(${name} ${version} COMPONENTS ${ARG_COMPONENTS} QUIET)
   else()
     find_package(${name} ${version} QUIET)
   endif()

   if(DEFINED ARG_MODULE)
     list(APPEND thirdparty ${ARG_MODULE})
   endif()

endmacro()

function(library_var_maker name)
    if(${name} MATCHES ${CMAKE_PROJECT_NAME})
        set(lib_include_dir "include" PARENT_SCOPE)
        set(lib_source_dir  "src" PARENT_SCOPE)
    else()
        set(lib_include_dir "include/${CMAKE_PROJECT_NAME}/${name}" PARENT_SCOPE)
        set(lib_source_dir  "src/${name}" PARENT_SCOPE)
    endif()
endfunction()

function(cppm_target_install)
    set(options BINARY STATIC SHARED INTERFACE INSTALL)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    list(GET ARG_UNPARSED_ARGUMENTS 0 name)
    library_var_maker(${name})
    target_link_libraries(${name} PUBLIC ${thirdparty})

    if(${ARG_BINARY})
        target_include_directories(${name}
            PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
    elseif(${ARG_STATIC} OR ${ARG_SHARED})
        target_include_directories(${name}
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${lib_include_dir}>
                $<INSTALL_INTERFACE:include>
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/${lib_source_dir}
        )
    elseif(${ARG_INTERFACE})
        target_include_directories(${name}
            INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${lib_include_dir}>
                $<INSTALL_INTERFACE:include>
        )
    endif()

    if(${ARG_BINARY} AND ${ARG_INSTALL})
        install(TARGETS ${name} RUNTIME DESTINATION bin)
    endif()
    if(${ARG_STATIC} OR ${ARG_SHARED} OR ${ARG_INTERFACE} AND ${ARG_INSTALL})
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
    endif()
endfunction()

function(download_package)
    set(options LOCAL GLOBAL)
    set(oneValueArgs URL GIT GIT_TAG)
    set(multiValueArgs CMAKE_ARGS W_CONFIGURE W_BUILD W_INSTALL
                                  L_CONFIGURE L_BUILD L_INSTALL)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    list(GET ARG_UNPARSED_ARGUMENTS 0 name)
    list(GET ARG_UNPARSED_ARGUMENTS 1 version)
    list(REMOVE_AT ARG_UNPARSED_ARGUMENTS 0 1)

    if(ARG_LOCAL)
      set(L_INSTALL_PREFIX "-DCMAKE_INSTALL_PREFIX=$ENV{HOME}/.cppm/local ")
      set(W_INSTALL_PREFIX "-DCMAKE_INSTALL_PREFIX=$ENV{HOME}\\.cppm\\local ")
    elseif(ARG_GLOBAL)
      set(_INSTALL_PREFIX "")
    else()
      message(FATAL_ERROR "Need Option LOCAL or GLOBAL")
    endif()

    if(${version} STREQUAL "lastest")
      set(version "")
    endif()

    include(ExternalProject)
    find_package(${name} ${version} QUIET)
    if(NOT "${${name}_FOUND}" AND NOT "${${name}_FIND_VERSION_EXACT}")
        message("-- [cppm] Can not find ${name} package")
        message("-- [cppm] Download ${name} package")
        #find_package(Git REQUIRED)

        if(NOT WIN32)
          ExternalProject_Add(
            ${name}
            URL ${ARG_URL}
            GIT_REPOSITORY ${ARG_GIT}
            GIT_TAG ${ARG_GIT_TAG}
            SOURCE_DIR $ENV{HOME}/.cppm/install
            CMAKE_ARGS ${CMAKR_ARGS} ${L_INSTALL_PREFIX} ${ARG_CMAKE_ARGS}
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
            SOURCE_DIR $ENV{HOME}\.cppm\install
            CMAKE_ARGS ${CMAKR_ARGS} ${W_INSTALL_PREFIX} ${ARG_CMAKE_ARGS}
            CONFIGURE_COMMAND ${ARG_W_CONFIGURE}
            BUILD_COMMAND ${ARG_W_BUILD}
            INSTALL_COMMAND ${ARG_W_INSTALL}
            BUILD_IN_SOURCE true
            ${ARG_UNPARSED_ARGUMENTS}
          )
        endif(NOT WIN32)
    else()
        message("-- [cppm] Find ${name} package")
    endif()
endfunction()

    # pkg-config install part
    
    # set(PKGCONFIG_INSTALL_DIR
    #    "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig"
    #    CACHE PATH "Path where ${CMAKE_PROJECT_NAME}.pc is installed"
    # )
    # configure_file(
    #  ${CMAKE_CURRENT_SOURCE_DIR}/CMake/${CMAKE_PROJECT_NAME}.pc.in
    #  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pc
    #  @ONLY
    # )
    # install(
    #  FILES
    #    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pc"
    #  DESTINATION
    #    ${PKGCONFIG_INSTALL_DIR}
    # )
    
      
    
    # set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
    # set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
    # set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
    # set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")
      
    # includedir=@CMAKE_INSTALL_FULL_INCLUDEDIR@
    # Name: Catch2
    # Description: A modern, C++-native, header-only, test framework for C++11
    # URL: https://github.com/catchorg/Catch2
    # Version: @Catch2_VERSION@
    # Cflags: -I${includedir}
    

    # prefix=@CMAKE_INSTALL_PREFIX@
    # exec_prefix=@CMAKE_INSTALL_PREFIX@
    # libdir=@INSTALL_LIB_DIR@
    # sharedlibdir=@INSTALL_LIB_DIR@
    # includedir=@INSTALL_INC_DIR@
    # 
    # Name: zlib
    # Description: zlib compression library
    # Version: @VERSION@
    # 
    # Requires:
    # Libs: -L${libdir} -L${sharedlibdir} -lz
    # Cflags: -I${includedir}
