find_package(nieel REQUIRED)

include(${CMAKE_MODULE_PATH}/utils.cmake)
#make_third_party_static_lib(nieel NIEEL)
list(APPEND third_party_library nieel)