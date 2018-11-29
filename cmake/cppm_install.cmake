set(cppm_project_dir "$ENV{HOME}/.${PROJECT_NAME}")
install(DIRECTORY DESTINATION ${cppm_project_dir}/cmake)
install(DIRECTORY DESTINATION ${cppm_project_dir}/repo)
install(DIRECTORY DESTINATION ${cppm_project_dir}/local)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/project_maker.cmake DESTINATION ${cppm_project_dir}/cmake)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/include/cppm_version.h")
