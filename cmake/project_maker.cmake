#include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)

function(build_binary name source dependencies)
    add_executable(${name} ${source})
    target_include_directories(${name}
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(${name} PUBLIC ${dependencies})
   # set_target_properties(${name} PROPERTIES LINKER_LANGUAGE CXX)
    
    include(GNUInstallDirs)
    install(TARGETS ${name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endfunction()

function(library_var_maker name)
    if(${name} MATCHES ${CMAKE_PROJECT_NAME})
        set(lib_include_dir "include" PARENT_SCOPE)
        set(lib_source_dir  "src" PARENT_SCOPE)
    else()
        set(lib_include_dir "include/${CMAKE_PROJECT_NAME}/${name}" PARENT_SCOPE)
        set(lib_source_dir  "src/${name}" PARENT_SCOPE)
    endif()
endfunction()

function(build_library name type source dependencies)
    library_var_maker(${name})

    if(${type} MATCHES "static")
        add_library(${name} STATIC ${source})
        target_link_libraries(${name} ${dependencies})
        set_target_properties(${name} PROPERTIES LINKER_LANGUAGE CXX)
        
    elseif(${type} MATCHES "header_only")
        add_library(${name} INTERFACE)
        target_link_libraries(${name} INTERFACE ${dependencies})
        
    else()
        add_library(${name} SHARED ${source}) 
        target_link_libraries(${name} ${dependencies})
        set_target_properties(${name} PROPERTIES LINKER_LANGUAGE CXX)
        
    endif()
    
    if(NOT ${name} MATCHES ${CMAKE_PROJECT_NAME})
        set_target_properties(${name} PROPERTIES PREFIX "lib${PROJECT_NAME}-")
    endif()
    
    if(${type} MATCHES "header_only")
        target_include_directories(${name}
            INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${lib_include_dir}>
        )
        target_include_directories(${name}
            SYSTEM INTERFACE 
                $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
        )
    else()
        target_include_directories(${name}
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${lib_include_dir}>
                $<INSTALL_INTERFACE:include>
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/${lib_source_dir}
        )
    endif()
    
    include(GNUInstallDirs)
    install(TARGETS ${name} EXPORT ${PROJECT_NAME}-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    
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
    # ===================================
    
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
    
    
    
    # ==================================
    
    
endfunction()
      
    
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

      
