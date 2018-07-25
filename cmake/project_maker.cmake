include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)

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
        message("${lib_include_dir}")
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
    
endfunction()
      
 #   include(CMakePackageConfigHelpers)
 #   configure_package_config_file(
 #       ${CMAKE_CURRENT_LIST_DIR}/CMake/Catch2Config.cmake.in
 #       ${CMAKE_CURRENT_BINARY_DIR}/Catch2Config.cmake
 #       INSTALL_DESTINATION
 #         ${CATCH_CMAKE_CONFIG_DESTINATION}
 #   )
 #   set(PKGCONFIG_INSTALL_DIR
 #       "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig"
 #       CACHE PATH "Path where catch2.pc is installed"
 #   )
 #   configure_file(
 #     ${CMAKE_CURRENT_SOURCE_DIR}/CMake/catch2.pc.in
 #     ${CMAKE_CURRENT_BINARY_DIR}/catch2.pc
 #     @ONLY
 #   )
 #   install(
 #     FILES
 #       "${CMAKE_CURRENT_BINARY_DIR}/catch2.pc"
 #     DESTINATION
 #       ${PKGCONFIG_INSTALL_DIR}
 #   ) 
      
      
      
      
     # if(${name} MATCHES ${PROJECT_NAME})
        #export(${name}Targets NAMESPACE ${name})
     
        
     # else()
     #   set_target_properties(${name} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_${name}")
     #   target_include_directories(${name}
     #       PUBLIC
     #           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${name}>
     #           $<INSTALL_INTERFACE:include/${name}>
     #       PRIVATE
     #           ${CMAKE_CURRENT_SOURCE_DIR}/src/${name}
     #   )
     #   
     #   include(GNUInstallDirs)
     #   install(TARGETS ${name} 
     #       EXPORT ${PROJECT_NAME}Targets-${name}
     #       ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${name}
     #       LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${name}
     #       RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${name}
     #   )
     #   install(DIRECTORY include/${name} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
     #   install(EXPORT ${PROJECT_NAME}Targets-${name}
     #       FILE ${PROJECT_NAME}Targets-${name}.cmake
     #       NAMESPACE ${PROJECT_NAME}::
     #       DESTINATION lib/cmake/${PROJECT_NAME}
     #   )
     #endif()
     
            
    #export(TARGETS ${name} FILE ${name}Config.cmake)
    #set_target_properties(${name} PROPERTIES
    #    VERSION ${${name}_VERSION}}
    #    SOVERSION 1
    #)
   # file(GLOB_RECURSE GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cpp"  
   #                               "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cc"
   #                               "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cxx"
   # )
