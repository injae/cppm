find_package(nieel COMPONENTS util REQUIRED)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utils.cmake)
#make_third_party_static_lib(nieel NIEEL)
list(APPEND third_party_library nieel)
