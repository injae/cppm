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