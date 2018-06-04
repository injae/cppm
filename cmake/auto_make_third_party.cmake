include(ExternalProject)

function(auto_make_third_party library_name)
    IF(EXISTS ${THIRD_PARTY_DIR}/${library_name}/CMakeLists.txt)
        cmake_project(${library_name})
    ELSE()
        non_cmake_project(${library_name})
    ENDIF()
endfunction()

function(cmake_project library_name)
    set(ROOT ${THIRD_PARTY_DIR}/${library_name})
    set(LIB_DIR ${${library_name}_ROOT}/bin/lib)
    set(INCLUDE_DIR ${${library_name}_ROOT}/bin/include)
    
    ExternalProject_Add(
       ${library_name}_
    
       #PREFIX ${THIRD_PARTY_DIR}/${library_name}
       SOURCE_DIR  "${ROOT}"
       INSTALL_DIR "${ROOT}/bin"
       CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
       BUILD_COMMAND make
       INSTALL_COMMAND make install
       BUILD_IN_SOURCE 1
    )
    
    ExternalProject_Get_Property(${library_name}_ install_dir)
    
#    ExternalProject_Add_Step(
#        ${library_name} CopyToBin
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/${library_name}/bin ${GLOBAL_OUTPUT_PATH}
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/${library_name}/lib ${GLOBAL_OUTPUT_PATH}
#        DEPENDEES install
#    )
endfunction()

function(non_cmake_project library_name)
    set(ROOT ${THIRD_PARTY_DIR}/${library_name})
    set(LIB_DIR ${${library_name}_ROOT}/bin/lib)
    set(INCLUDE_DIR ${${library_name}_ROOT}/bin/include)
    
    ExternalProject_Add(
       ${library_name}_
       
       PREFIX ${ROOT}
       SOURCE_DIR  "${ROOT}"
       INSTALL_DIR "${ROOT}/bin"
       BUILD_IN_SOURCE 1
       BUILD_COMMAND make
       INSTALL_COMMAND make install
       CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --prefix=${ROOT}/bin
    )
    
#    ExternalProject_Add_Step(
#        ${library_name} CopyToBin
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/${library_name}/bin ${GLOBAL_OUTPUT_PATH}
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/${library_name}/lib ${GLOBAL_OUTPUT_PATH}
#        DEPENDEES install
#    )
endfunction()



#ExternalProject_Add(
#   ${library_name}_
#
#   GIT_REPOSITORY  "https://.git"
#   BINARY_DIR  "${ROOT}"
#   SOURCE_DIR  "${ROOT}"
#   INSTALL_DIR "${ROOT}/bin"
#   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
#   BUILD_COMMAND make
#   BUILD_IN_SOURCE 1
#   INSTALL_COMMAND make install
#   CONFIGURE_COMMAND "${ROOT}/configure" --prefix=${GLOBAL_OUTPUT_PATH}/protobuf
#)
#
#ExternalProject_Add_Step(
#    HumbleLogging CopyToBin
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/${library_name}/bin ${GLOBAL_OUTPUT_PATH}
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/${library_name}/lib ${GLOBAL_OUTPUT_PATH}
#    DEPENDEES install
#)