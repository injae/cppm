include(${CMAKE_MODULE_PATH}/utils.cmake)

function(install_library name)
    install_library_config(${name})
endfunction()

function(make_excutable name)
    file(GLOB_RECURSE GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"  
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    )
    list(APPEND SOURCES ${GLOB_RESULT})
    get_third_party_list()
    add_executable(${name} ${SOURCES})
    target_include_directories(${name}
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(${name} PUBLIC ${third_party_library})
    
    include(GNUInstallDirs) 
    install(TARGETS ${name} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endfunction()

function(make_lib name)
    get_third_party_list()
    
    file(GLOB_RECURSE GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"  
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    )
    list(APPEND SOURCES ${GLOB_RESULT})
    
    if(BUILD_SHARED_LIB) 
        add_library(${name} SHARED ${SOURCES})
        target_link_libraries(${name} ${third_party_library})
    else()
        add_library(${name} STATIC ${SOURCES})
        target_link_libraries(${name} ${third_party_library})
    endif()
    unset(BUILD_SHARED_LIB CACHE)
        
    install_library(${name})
endfunction()

function(make_static_shared_lib name)
    get_third_party_list()
    
    file(GLOB_RECURSE GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"  
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    )
    list(APPEND SOURCES ${GLOB_RESULT})
    
    add_library(${name} SHARED ${SOURCES})
    add_library(${name}-static STATIC ${SOURCES})
    
    set_target_properties(${name}-static PROPERTIES OUTPUT_NAME ${name}) 
    target_link_libraries(${name} ${third_party_library})
    target_link_libraries(${name}-static ${third_party_library})
    
    install_library(${name})
    install_library(${name}-static)
endfunction()

function(make_static_lib name)
    get_third_party_list()
    
    file(GLOB_RECURSE GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"  
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    )
    list(APPEND SOURCES ${GLOB_RESULT})
    
    add_library(${name} STATIC ${SOURCES})
    
    target_link_libraries(${name} ${third_party_library})
    install_library(${name})
endfunction()

function(make_shared_lib name)
    get_third_party_list()
    
    file(GLOB_RECURSE GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"  
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    )
    list(APPEND SOURCES ${GLOB_RESULT})
    
    add_library(${name} SHARED ${SOURCES})
    
    target_link_libraries(${name} ${third_party_library})
    install_library(${name})
endfunction()

function(make_header_only_lib name)
    get_third_party_list()
    
    add_library(${name} INTERFACE)
    target_include_directories(${name} INTERFACE ${path})
    
    target_link_libraries(${name} ${third_party_library})
    install_library(${name})
endfunction()

function(install_library_config name)
    include(GNUInstallDirs) 
    target_include_directories(${name} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    
    #set_target_properties(${name} PROPERTIES
    #    VERSION ${${PROJECT_NAME}_VERSION}}
    #    SOVERSION 1
    #)
    
    install(TARGETS ${name} EXPORT ${name}Config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${name})

    install(EXPORT ${name}Config DESTINATION lib/cmake/${name})
    
    export(TARGETS ${name} FILE ${name}Config.cmake)
endfunction()

MACRO(make_third_party_shared_lib name third_party_lib)
    message("${${third_party_lib}_INCLUDE_DIR}")
    
    if(${third_party_lib}_FOUND)
        add_library(${name} SHARED IMPORTED)
        set_target_properties(${name} PROPERTIES IMPORTED_LOCATION ${${third_party_lib}_LIBRARIES})
        set_target_properties(${name} PROPERTIES INCLUDE_DIRECTORIES ${${third_party_lib}_INCLUDE_DIR})
    else()
        
    endif()
        
ENDMACRO()

MACRO(make_third_party_static_lib name third_party_lib)
    if(${third_party_lib}_FOUND)
        add_library(${name} STATIC IMPORTED)
        set_target_properties(${name} PROPERTIES IMPORTED_LOCATION ${${third_party_lib}_LIBRARIES})
        set_target_properties(${name} PROPERTIES INCLUDE_DIRECTORIES ${${third_party_lib}_INCLUDE_DIR})
        message("linked thirddparty ${name}")
    else()
        
    endif()
        
ENDMACRO()

MACRO(make_third_party_header_only_lib name path)
    add_library(${name} INTERFACE)
    target_include_directories(${name} INTERFACE ${path})
ENDMACRO()

#function(install_library_config name)
#    target_include_directories(${name} PUBLIC
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#        $<INSTALL_INTERFACE:include>
#        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
#    )
#        
#    install(TARGETS ${name} EXPORT ${name}Targets
#        ARCHIVE  DESTINATION lib
#        LIBRARY  DESTINATION lib 
#        RUNTIME  DESTINATION bin 
#        INCLUDES DESTINATION include 
#    )
#    
#    install(EXPORT ${name}Targets
#        FILE ${name}Targets.cmake
#        NAMESPACE ${name}::
#        DESTINATION lib/cmake/${name}
#    )
#    
#    #install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#    install_library_include(${name} ${INCLUDE_DIR})
#    
#    install(EXPORT ${name}Targets DESTINATION static/${name}/cmake)
#    
#    include(CMakePackageConfigHelpers)
#    write_basic_package_version_file("${name}ConfigVersion.cmake"
#        VERSION ${name}_Version}
#        COMPATIBILITY SameMajorVersion
#    )
#    
#    install(FILES "${name}Config.cmake" "${name}ConfigVersion.cmake"
#        DESTINATION lib/cmake/${name}
#    )
#    
#function(install_library_include name include_dir)
#    subdirectory_list_f(folder_list ${include_dir})
#    list(APPEND folder_list ${include_dir})
#    foreach(folder ${folder_list}) 
#        file_list(headers ${folder})
#        string(REPLACE "${include_dir}" "" buffer ${folder})
#        install(FILES ${headers} DESTINATION "include/${name}/${buffer}")  
#    endforeach()
#endfunction()
#endfunction()