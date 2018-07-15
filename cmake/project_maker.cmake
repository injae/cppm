include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)
function(cpp_file_list path)
    file(GLOB_RECURSE  GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cpp"  
                                   "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cc"
                                   "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cxx"
    )
    set(source ${GLOB_RESULT} PARENT_SCOPE)
endfunction()

function(build_binary name source dependencies)
    add_executable(${name} ${source})
    target_include_directories(${name}
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(${name} PUBLIC ${dependencies})
    #set_target_properties(${name} PROPERTIES LINKER_LANGUAGE CXX)
    
    include(GNUInstallDirs)
    install(TARGETS ${name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endfunction()

function(build_library name type source dependencies)
    if(type MATCHES "static")
        add_library(${name} STATIC ${source})
        target_link_libraries(${name} ${dependencies})
    else()
        add_library(${name} SHARED ${source}) 
        target_link_libraries(${name} ${dependencies})
    endif()
    set_target_properties(${name} PROPERTIES LINKER_LANGUAGE CXX)
    
    if(${name} MATCHES ${PROJECT_NAME})
        target_include_directories(${name}
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
        include(GNUInstallDirs)
        install(TARGETS ${name} EXPORT ${name}Targets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
        install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${name})
        install(EXPORT ${name}Targets
            FILE ${name}Targets.cmake
            NAMESPACE ${name}::
            DESTINATION lib/cmake/${name}
        )
        #export(${name}Targets NAMESPACE ${name})
        
        
        
        
    else()
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
    endif()
endfunction()
     
            
    #export(TARGETS ${name} FILE ${name}Config.cmake)
    #set_target_properties(${name} PROPERTIES
    #    VERSION ${${name}_VERSION}}
    #    SOVERSION 1
    #)
   # file(GLOB_RECURSE GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cpp"  
   #                               "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cc"
   #                               "${CMAKE_CURRENT_SOURCE_DIR}/${path}*.cxx"
   # )