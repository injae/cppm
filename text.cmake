
#=====================================================================	   
# install variable setting
#=====================================================================	   
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(target_export_name ${name}-targets)
set(version_config     ${PROJECT_BINARY_DIR}/${name}-config-version.cmake)
set(project_config     ${PROJECT_BINARY_DIR}/${name}-config.cmake)
set(target_config      ${target_export_name}.cmake)
set(cmake_install_dir  ${CMAKE_INSTALL_LIBDIR}/cmake/${name})
#=====================================================================	   
# ${name}-config-version.cmake generate
#=====================================================================	   
write_basic_package_version_file(
	${version_config}
	VERSION ${${CMAKE_PROJECT_NAME}_VERSION}
	COMPATIBILITY AnyNewerVersion	
) 
install(FILES ${version_config} DESTINATION ${cmake_install_dir})
#=====================================================================
# ${name}-config.cmake generate
#=====================================================================
file(WRITE ${PROJECT_SOURCE_DIR}/cmake/${name}-config.cmake.in
	"@PACKAGE_INIT@\n"
	"include(${CMAKE_CURRENT_LIST_DIR}/@targets_export_name@.cmake)\n"
	"check_required_components(${name})\n"
)	
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/${name}-config.cmake.in
    ${project_config}
    INSTALL_DESTINATION ${cmake_install_dir}
)
install(FILES ${project_config} DESTINATION ${cmake_install_dir})
#====================================================================
# ${name}-targets.cmake generate
#=====================================================================	   
install(TARGETS ${name} EXPORT ${target_export_name}
	    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
	    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
	    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
	    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${target_export_name}
	    FILE ${target_config}
	    NAMESPACE ${CMAKE_PROJECT_NAME}::
	    DESTINATION ${cmake_install_dir}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#=====================================================================	   
