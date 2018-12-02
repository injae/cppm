set(cppm_project_dir "$ENV{HOME}/.${PROJECT_NAME}")
file(MAKE_DIRECTORY ${cppm_project_dir}/cmake)
file(MAKE_DIRECTORY ${cppm_project_dir}/repo)
file(MAKE_DIRECTORY ${cppm_project_dir}/local)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/project_maker.cmake DESTINATION ${cppm_project_dir}/cmake )

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/include/cppm_version.h")
