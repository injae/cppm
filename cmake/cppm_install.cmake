file(MAKE_DIRECTORY ${CPPM_CORE}/cmake)
file(MAKE_DIRECTORY ${CPPM_CORE}/repo)
file(MAKE_DIRECTORY ${CPPM_CORE}/share)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cppm_tool.cmake      DESTINATION ${CPPM_MODULE})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/HunterGate.cmake     DESTINATION ${CPPM_MODULE})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/include/cppm_version.h")

include(${CPPM_CORE}/cppkg/updater.cmake)
