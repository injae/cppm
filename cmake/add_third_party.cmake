include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/auto_make_third_party.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)

MACRO(add_thirdparty_library library_name dependcy_lib)
    MESSAGE("================================")
    MESSAGE("-- ${library_name}")
    message("-- Found the following ${library_name} libraries") 
    
    SET(root_dir    "${THIRD_PARTY_DIR}/${library_name}" )
    SET(include_dir "${root_dir}/bin/include")
    SET(library_dir "${root_dir}/bin/lib")
    
    auto_make_third_party(${library_name})
    file_list(files_ ${library_dir})
    foreach(lib ${files_})
        string(REPLACE "." ";" find_lib ${lib})
        list(GET find_lib 0 library_path_)
        string(REPLACE "${library_dir}/lib" "" library_name_ ${library_path_})
        list(GET find_lib 1 library_type_)
        
        set(is_exist false)
        foreach(li ${third_party_library})
            if(${li} MATCHES ${library_name_})
                set(is_exist true)
            endif()
        endforeach()
        IF(NOT ${is_exist} MATCHES true)
            IF(${library_type_} MATCHES "a")      # static
                add_library(${library_name_} STATIC IMPORTED)
                IF(${dependcy_lib} MATCHES none)
                    set_target_properties(${library_name_} PROPERTIES IMPORTED_LOCATION ${lib})
                ELSE()
                    set_target_properties(${library_name_} PROPERTIES IMPORTED_LOCATION ${lib}
                                          IMPORTED_LINK_INTERFACE_LIBRARIES ${dependcy_lib})
                ENDIF()
                
            ELSEIF(${library_type_} MATCHES "so") # shared
                add_library(${library_name_} SHARED IMPORTED)
                IF(${dependcy_lib} MATCHES none)
                    set_target_properties(${library_name_} PROPERTIES IMPORTED_LOCATION ${lib})
                ELSE()
                    set_target_properties(${library_name_} PROPERTIES IMPORTED_LOCATION ${lib}
                                          IMPORTED_LINK_INTERFACE_LIBRARIES ${dependcy_lib})
                ENDIF()
            ENDIF()
            
            list(APPEND third_party_library ${library_name_})
            #list(APPEND third_party_library ${lib})
            include_directories(SYSTEM ${include_dir}) 
            #message("${third_party_library}")
        ENDIF()    
    endforeach()
    MESSAGE("================================")
    
ENDMACRO()


#MACRO(add_third_party_header_only_lib library_name)
#    message("================================")
#    message("-- ${library_name}")
#    message("-- Found the following ${library_name} libraries") 
#    
#    set(${library_name}_root_dir "${THIRD_PARTY_DIR}/${library_name}" )
#    set(${library_name}_include_dir "${${library_name}_root_dir}/bin/include")
#    set(${library_name}_library_dir "${${library_name}_root_dir}/bin/lib")
#    
#    get_subdirectory_list(list ${${library_name}_include_dir})
#    foreach(subdir ${list})
#        message("--   ${subdir}")
#        include_directories(${${library_name}_include_dir}/) 
#    endforeach()
#    
#    message("================================")
#    
#ENDMACRO()


#IF(NOT EXISTS ${library_dir})
#    auto_make_third_party(${library_name})
#ENDIF()
#
#get_subdirectory_list(list ${include_dir})
#FOREACH(subdir ${list})
#    #message("--   ${subdir}")
#    
#    FILE(GLOB find_lib "${library_dir}/lib${subdir}*")
#    
#    IF(find_lib MATCHES ".a")      # static
#        add_library(${subdir} STATIC IMPORTED)
#        IF(${dependcy_lib} MATCHES none)
#            set_target_properties(${subdir} PROPERTIES IMPORTED_LOCATION ${library_dir}/lib${subdir}.a)
#        ELSE()
#            set_target_properties(${subdir} PROPERTIES IMPORTED_LOCATION ${library_dir}/lib${subdir}.a
#                                  IMPORTED_LINK_INTERFACE_LIBRARIES "${depency_lib}")
#        ENDIF()
#        
#    ELSEIF(find_lib MATCHES ".so") # shared
#        add_library(${subdir} SHARED IMPORTED)
#        IF(${dependcy_lib} MATCHES none)
#            set_target_properties(${subdir} PROPERTIES IMPORTED_LOCATION ${library_dir}/lib${subdir}.so)
#        ELSE()
#            set_target_properties(${subdir} PROPERTIES IMPORTED_LOCATION ${library_dir}/lib${subdir}.so
#                                  IMPORTED_LINK_INTERFACE_LIBRARIES "${depency_lib}")
#        ENDIF()
#    ENDIF()
#    include_directories(SYSTEM ${include_dir}/) 
#ENDFOREACH()
#list(APPEND ${third_party_library} ${library_name})
#MESSAGE("================================")