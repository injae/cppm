function(subdirectory_list_f result target_dir)
    file(GLOB children RELATIVE ${target_dir} ${target_dir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${target_dir}/${child})
            subdirectory_list_f(dirlist ${target_dir}/${child})
            list(APPEND dirlist ${target_dir}/${child})
            #message("${dirlist}")
        endif()
    endforeach()
    set(${result} ${dirlist} PARENT_SCOPE)
endfunction()

function(subdirectory_list result target_dir)
    file(GLOB children RELATIVE ${target_dir} ${target_dir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${target_dir}/${child})
            get_subdirectory_list(dirlist ${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist} PARENT_SCOPE)
endfunction()

function(file_list result_ target_dir_)
    file(GLOB files ${target_dir_}/*)
    set(filelist "")
    foreach(file_ ${files})
        if(NOT IS_DIRECTORY ${file_}) 
            list(APPEND filelist ${file_})
        endif()
    endforeach()
    set(${result_} ${filelist} PARENT_SCOPE)
endfunction()

macro(get_third_party_list)
    file(GLOB_RECURSE find_file "${MODULE_PATH}/*.cmake")
    FOREACH(file ${find_file})
        include(${file})
    ENDFOREACH()
endmacro()

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
    install_library(${name} ${INCLUDE_DIR})
endfunction()

function(make_shared_ib name)
    get_third_party_list()
    
    file(GLOB_RECURSE GLOB_RESULT "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"  
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
                                  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    )
    list(APPEND SOURCES ${GLOB_RESULT})
    
    add_library(${name} SHARED ${SOURCES})
    target_link_libraries(${name} ${third_party_library})
    install_library(${name} ${INCLUDE_DIR})
endfunction()

function(install_library_config name)
    target_include_directories(${name} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
        
    install(TARGETS ${name} EXPORT ${name}Targets
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib 
        RUNTIME  DESTINATION bin 
        INCLUDES DESTINATION include 
    )
    
    install(EXPORT ${name}Targets
        FILE ${name}Targets.cmake
        NAMESPACE ${name}::
        DESTINATION lib/cmake/${name}
    )
    
    #install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install_library_include(${name} ${INCLUDE_DIR})
    
    install(EXPORT ${name}Targets DESTINATION static/${name}/cmake)
    
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file("${name}ConfigVersion.cmake"
        VERSION ${name}_Version}
        COMPATIBILITY SameMajorVersion
    )
    
    install(FILES "${name}Config.cmake" "${name}ConfigVersion.cmake"
        DESTINATION lib/cmake/${name}
    )
    
    #include(CMakeFindDependencyMacro)
    #find_dependency(Boost 1.5.4)
    #include(${CMAKE_CURRENT_LIST_DIR}/${name}Target.cmake")
endfunction()

function(install_library_include name include_dir)
    subdirectory_list_f(folder_list ${include_dir})
    list(APPEND folder_list ${include_dir})
    foreach(folder ${folder_list}) 
        file_list(headers ${folder})
        string(REPLACE "${include_dir}" "" buffer ${folder})
        install(FILES ${headers} DESTINATION "include/${name}/${buffer}")  
    endforeach()
endfunction()

function(install_library include_dir)
    install_library_config()
    install_library_include(${include_dir})
endfunction()

MACRO(make_third_party_shared_lib name third_party_lib)
    
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