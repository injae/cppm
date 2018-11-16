
set(cppm_project_dir "$ENV{HOME}/.${PROJECT_NAME}")
install(DIRECTORY DESTINATION ${cppm_project_dir}/cmake)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/project_maker.cmake DESTINATION ${cppm_project_dir}/cmake)
#execute_process(COMMAND mkdir "${cppm_project_dir}")
#execute_process(COMMAND mkdir "${cppm_project_dir}/cmake")
#execute_process(COMMAND copy "${CMAKE_CURRENT_SOURCE_DIR}/cmake/project_maker.cmake ${cppm_project_dir}/cmake")

